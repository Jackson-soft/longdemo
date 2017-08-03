#include "Context.h"

Context::Context() { m_pStrategy = nullptr; }

Context::~Context() {}

void Context::algorithm() { m_pStrategy->algorithm(); }

void Context::setStrategy(Strategy *st) { m_pStrategy = st; }
