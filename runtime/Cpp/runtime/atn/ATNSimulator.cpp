﻿/*
 * [The "BSD license"]
 *  Copyright (c) 2016 Mike Lischke
 *  Copyright (c) 2013 Terence Parr
 *  Copyright (c) 2013 Dan McLaughlin
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ATNType.h"
#include "ATNConfigSet.h"
#include "DFAState.h"
#include "ATNDeserializer.h"
#include "EmptyPredictionContext.h"

#include "ATNSimulator.h"

using namespace org::antlr::v4::runtime::dfa;
using namespace org::antlr::v4::runtime::atn;

const std::shared_ptr<DFAState> ATNSimulator::ERROR = std::make_shared<DFAState>(INT32_MAX);

ATNSimulator::ATNSimulator() {
}

ATNSimulator::ATNSimulator(const ATN &atn, std::shared_ptr<PredictionContextCache> sharedContextCache)
: atn(atn), _sharedContextCache(sharedContextCache) {
}

std::shared_ptr<PredictionContextCache> ATNSimulator::getSharedContextCache() {
  return _sharedContextCache;
}

PredictionContextRef ATNSimulator::getCachedContext(PredictionContextRef context) {
  std::lock_guard<std::mutex> lck(mtx);
  std::map<PredictionContextRef, PredictionContextRef> visited;
  return PredictionContext::getCachedContext(context, _sharedContextCache, visited);
}

ATN ATNSimulator::deserialize(const std::wstring &data) {
  ATNDeserializer deserializer;
  return deserializer.deserialize(data);
}

void ATNSimulator::checkCondition(bool condition) {
  (new ATNDeserializer())->checkCondition(condition);
}

void ATNSimulator::checkCondition(bool condition, const std::string &message) {
  ATNDeserializer().checkCondition(condition, message);
}

Transition *ATNSimulator::edgeFactory(const ATN &atn, int type, int src, int trg, int arg1, int arg2, int arg3,
                                      const std::vector<misc::IntervalSet> &sets) {
  return (new ATNDeserializer())->edgeFactory(atn, type, src, trg, arg1, arg2, arg3, sets);
}

ATNState *ATNSimulator::stateFactory(int type, int ruleIndex) {
  return (new ATNDeserializer())->stateFactory(type, ruleIndex);
}
