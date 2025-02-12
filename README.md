# TracerBench

In this example, we will demonstrate how to effectively use LTTng (Linux Trace
Toolkit Next Generation) and run benchmarks to measure the performance impact of
adding tracepoints to our software. This project uses the [SeeMake](https://github.com/MhmRhm/SeeMake)
template. Follow the instructions in the template to build this project.

To install LTTng, run the following commands:

```bash
sudo apt-get install lttng-tools
sudo apt-get install liblttng-ust-dev
```

Our example involves generating random numbers and determining which numbers are
prime. We have two types of threads: a producer that generates random numbers and
a consumer that processes these numbers to check for primes. We have added
tracepoints at the entry and exit of each function. Additionally, we have a
tracepoint that records whether each number is prime.

First, let's dive into the benchmarking results. Afterward, I will explain how to
use LTTng command line tools to start a recording session and capture tracepoint
events that your code is linked against. Finally, I will walk you through the
code and explain how everything works.

## Benchmarking Results

I conducted multiple runs of this application with and without tracepoints
enabled to measure the overhead introduced by tracing.

- **Without Tracepoints**: The application processed 37,337,667 numbers in 10
seconds and found 1,765,804 primes.
- **Without Recording**: With tracepoints present but no recording session
started, the application processed 37,723,333 numbers in 10 seconds and found
1,785,548 primes.
- **With Recording**: With tracepoints present and recording sessions started,
the application processed 36,197,333 numbers in 10 seconds and found 1,713,634
primes.

From the results, we can observe that the presence of tracepoints without
recording introduces an unexpected slight performance gain of 1%. However,
starting a recording session introduces an overhead of approximately 4%, which is
expected due to the additional processing required to record tracepoint events.

The key takeaway is that you only incur this 4% performance drop when a recording
session is active. During normal operation, without recording, the performance
impact is negligible. Additionally, this benchmarking experiment represents an
extreme case, as we record a trace for each number processed. In a more realistic
scenario, where fewer tracepoints are recorded, the performance impact would be
even less.

## LTTng Command Line

The following summarizes how you can start a recording session and view the
traces:

```bash
# To start the LTTng session daemon (run this once)
lttng-sessiond --daemonize
# To list available userspace tracepoints (requires the program to be running)
lttng list --userspace
# To create a new tracing session
lttng create user-space-session
# To enable a specific userspace tracepoint
lttng enable-event --userspace provider:event
# To start tracing
lttng start
# To destroy the session (ends session)
lttng destroy

# To display recorded events
babeltrace2 ~/lttng-traces/user-space-session*
```

You only need to run `lttng-sessiond --daemonize` once. After that, you can
create multiple recording sessions with `lttng create`. Note that your program
should be running if you want to list available userspace trace events with
`lttng list`. However, you can enable trace events with
`lttng enable-event --userspace` even before running your program.

To enable the tracepoint events for this application, run the following commands
as root:

```bash
sudo su -
lttng-sessiond --daemonize
lttng create user-space-session
lttng enable-event --userspace demo_app:prime_number_tracepoint
lttng enable-event --userspace demo_app:function_entry_tracepoint
lttng enable-event --userspace demo_app:function_exit_tracepoint
lttng start
```

## LTTng Tracepoint Events

Before adding tracepoints to your code, you need to define the trace events that
you will use in your program. You will define a provider name for your tracepoint
event and an event name. You should also define how many arguments your
tracepoint event takes and of which type they are. For example, to trace whether
each number is a prime or not, I have defined the following tracepoint event:

```c
LTTNG_UST_TRACEPOINT_EVENT(
    demo_app, prime_number_tracepoint,
    LTTNG_UST_TP_ARGS(unsigned int, number, bool, is_prime),
    LTTNG_UST_TP_FIELDS(
        lttng_ust_field_integer(unsigned int, number_field, number)
        lttng_ust_field_integer(bool, is_prime_field, is_prime)
    )
)
```

This tracepoint event takes two arguments: one integer which is the random
number, and the other a boolean indicating whether it is a prime number.

The following shows the use of this tracepoint event in the code:

```cpp
bool prime{isPrime(*itr)};
lttng_ust_tracepoint(demo_app, prime_number_tracepoint, *itr, prime);
if (prime) 
   count += 1;
```

The most effective debugging technique I have ever used to figure out the
multithreading issues in my code is actually a very basic one but taken to the
extreme. The following two tracepoint events collect a string (`const char *`):

```c
LTTNG_UST_TRACEPOINT_EVENT(demo_app, function_entry_tracepoint,
                           LTTNG_UST_TP_ARGS(const char *, function_name),
                           LTTNG_UST_TP_FIELDS(lttng_ust_field_string(
                               function_name_field, function_name)))

LTTNG_UST_TRACEPOINT_EVENT(demo_app, function_exit_tracepoint,
                           LTTNG_UST_TP_ARGS(const char *, function_name),
                           LTTNG_UST_TP_FIELDS(lttng_ust_field_string(
                               function_name_field, function_name)))
```

I have used them in the constructor and destructor of a class:

```cpp
class Tracer {
public:
  Tracer(const char *name) : functionName{name} {
    lttng_ust_tracepoint(demo_app, function_entry_tracepoint, functionName);
  }
  ~Tracer() {
    lttng_ust_tracepoint(demo_app, function_exit_tracepoint, functionName);
  }

private:
  const char *functionName;
};
```

At the entry point of each function in my code, I have constructed one object of
this class with the function name passed to it. This way, when there is a
deadlock or a very hard-to-debug timing issue in my multithreaded design, I can
easily look at the sequence of function calls and pinpoint the issue.

```cpp
Page createNumbers() {
  Tracer tracer(__func__);
  Page page{};
  ...
  return page;
}

void producer() {
  Tracer tracer(__func__);
  while (!done) {
    ...
  }
}

void consumer() {
  Tracer tracer(__func__);
  while (!done) {
    ...
  }
}
```

Following shows a sample recording session:

<p align="center"><img src="https://i.postimg.cc/3N5TZQBm/temp-Image9c-QBu-G.avif" alt="Documentation"></img></p>

You can see which CPU each function is called by and, using the timestamps, how
long it took to complete each function.
