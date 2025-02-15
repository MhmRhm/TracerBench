#pragma once

class CallTracer {
public:
  CallTracer(const char *name);
  ~CallTracer();

private:
  const char *functionName;
};