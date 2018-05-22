/*
 * Copyright (C) 2018 Roman Vaivod
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "extracearguments.h"

const string & ExtraceArguments::getMountPoint() const {
  return myMountPoint;
}

bool ExtraceArguments::hasUssLimit() const {
  return myUssLimit != UINT64_MAX;
}

uint64_t ExtraceArguments::getUssLimit() const {
  return myUssLimit;
}

int ExtraceArguments::getCpuLimit() const {
  return myCpuLimit;
}

void ExtraceArguments::setUssLimit(uint64_t rssLimit) {
  myUssLimit = rssLimit;
}

void ExtraceArguments::setCpuLimit(int cpuLimit) {
  myCpuLimit = cpuLimit;
}

void ExtraceArguments::setPIDs(const vector<int> & pids) {
  m_PIDs = pids;
}

bool ExtraceArguments::hasPIDs() const {
  return !m_PIDs.empty();
}

const vector<int> & ExtraceArguments::getPIDs() const {
  return m_PIDs;
}

void ExtraceArguments::setPeriod(uint32_t period) {
  m_Period = period;
}

bool ExtraceArguments::hasPeriod() const {
  return m_Period != 0;
}

uint32_t ExtraceArguments::getPeriod() const {
  return m_Period;
}

void ExtraceArguments::setTimes(uint32_t times) {
  m_Times = times;
}

bool ExtraceArguments::hasTimes() const {
  return m_Times != 0;
}

uint32_t ExtraceArguments::getTimes() const {
  return m_Times;
}

void ExtraceArguments::setAppName(const string & appName) {
  m_AppName = appName;
}

void ExtraceArguments::setHelpMessage(const string & message) {
  m_HelpMessage = message;
}

void ExtraceArguments::enableCircleBuffer() {
  m_CircleBuffer = true;
}

void ExtraceArguments::enableIgnoreSignals() {
  m_IgnoreSignals = true;
}

void ExtraceArguments::enableCompression() {
  m_Compression = true;
}

void ExtraceArguments::enableAsyncStart() {
  m_AsyncStart = true;
}

void ExtraceArguments::enableAsyncStop() {
  m_AsyncStop = true;
}

void ExtraceArguments::enableAsyncDump() {
  m_AsyncDump = true;
}

void ExtraceArguments::enableStream() {
  m_Stream = true;
}

void ExtraceArguments::enableListCategories() {
  m_ListCategories = true;
}

void ExtraceArguments::enableCoreServices() {
  m_CoreServices = true;
}

void ExtraceArguments::enableInterpret() {
  m_Interpret = true;
}

void ExtraceArguments::setOutputFilename(const string & filename) {
  m_OutputFilename = filename;
}

void ExtraceArguments::setBufferSize(uint32_t size) {
  m_BufferSize = size;
}

void ExtraceArguments::setInitSleepDuration(uint32_t duration) {
  m_InitSleepDuration = duration;
}

void ExtraceArguments::setMidSleepDuration(uint32_t duration) {
  m_MidSleepDuration = duration;
}

void ExtraceArguments::setApps(const vector<string> & apps) {
  m_Apps = apps;
}

void ExtraceArguments::addAndroidCategory(const Android::TraceCategory & traceCategory) {
  m_AndroidCategories.push_back(traceCategory);
}

void ExtraceArguments::addKernelCategory(const KernelTraceSystem::TraceCategory & traceCategory) {
  m_KernelCategories.push_back(traceCategory);
}

void ExtraceArguments::setKernelFunctions(const vector<string> & functions) {
  m_KernelFunctions = functions;
}

bool ExtraceArguments::hasHelpMessage() const {
  return !m_HelpMessage.empty();
}
bool ExtraceArguments::hasOutputFilename() const {
  return !m_OutputFilename.empty();
}
bool ExtraceArguments::hasApps() const {
  return !m_Apps.empty();
}
bool ExtraceArguments::hasAndroidCategories() const {
  return !m_AndroidCategories.empty();
}
bool ExtraceArguments::hasKernelCategories() const {
  return !m_KernelCategories.empty();
}
bool ExtraceArguments::hasKernelFunctions() const {
  return !m_KernelFunctions.empty();
}

bool ExtraceArguments::hasInitSleepDuration() const {
  return m_InitSleepDuration != 0;
}

bool ExtraceArguments::hasMidSleepDuration() const {
  return m_MidSleepDuration != 0;
}

bool ExtraceArguments::circleBufferEnabled() const {
  return m_CircleBuffer;
}

bool ExtraceArguments::ignoreSignalsEnabled() const {
  return m_IgnoreSignals;
}

bool ExtraceArguments::compressionEnabled() const {
  return m_Compression;
}

bool ExtraceArguments::asyncStartEnabled() const {
  return m_AsyncStart;
}

bool ExtraceArguments::asyncStopEnabled() const {
  return m_AsyncStop;
}

bool ExtraceArguments::asyncDumpEnabled() const {
  return m_AsyncDump;

}
bool ExtraceArguments::streamEnabled() const {
  return m_Stream;
}

bool ExtraceArguments::listCategoriesEnabled() const {
  return m_ListCategories;
}

bool ExtraceArguments::coreServicesEnabled() const {
  return m_CoreServices;
}

bool ExtraceArguments::interpretEnabled() const {
  return m_Interpret;
}

const string & ExtraceArguments::getOutputFilename() const {
  return m_OutputFilename;
}

const string & ExtraceArguments::getAppName() const {
  return m_AppName;
}

uint32_t ExtraceArguments::getBufferSize() const {
  return m_BufferSize;
}

uint32_t ExtraceArguments::getInitSleepDuration() const {
  return m_InitSleepDuration;
}

uint32_t ExtraceArguments::getMidSleepDuration() const {
  return m_MidSleepDuration;
}

const vector<string> & ExtraceArguments::getApps() const {
  return m_Apps;
}

const vector<Android::TraceCategory> & ExtraceArguments::getAndroidCategories() const {
  return m_AndroidCategories;
}

const vector<KernelTraceSystem::TraceCategory> & ExtraceArguments::getKernelCategories() const {
  return m_KernelCategories;
}

const vector<string> & ExtraceArguments::getKernelFunctions() const {
  return m_KernelFunctions;
}

const string & ExtraceArguments::getHelpMessage() const {
  return m_HelpMessage;
}

ExtraceArguments::ExtraceArguments() {
}
