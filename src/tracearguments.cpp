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

#include "tracearguments.h"

void TraceArguments::setAppName(const string & appName) {
  m_AppName = appName;
}

void TraceArguments::setHelpMessage(const string & message) {
  m_HelpMessage = message;
}

void TraceArguments::enableCircleBuffer() {
  m_CircleBuffer = true;
}

void TraceArguments::enableIgnoreSignals() {
  m_IgnoreSignals = true;
}

void TraceArguments::enableCompression() {
  m_Compression = true;
}

void TraceArguments::enableAsyncStart() {
  m_AsyncStart = true;
}

void TraceArguments::enableAsyncStop() {
  m_AsyncStop = true;
}

void TraceArguments::enableAsyncDump() {
  m_AsyncDump = true;
}

void TraceArguments::enableStream() {
  m_Stream = true;
}

void TraceArguments::enableListCategories() {
  m_ListCategories = true;
}

void TraceArguments::enableCoreServices() {
  m_CoreServices = true;
}

void TraceArguments::setKernelCategoryFilename(const string & filename) {
  m_KernelCategoryFilename = filename;
}

void TraceArguments::setOutputFilename(const string & filename) {
  m_OutputFilename = filename;
}

void TraceArguments::setBufferSize(uint32_t size) {
  m_BufferSize = size;
}

void TraceArguments::setInitSleepDuration(uint32_t duration) {
  m_InitSleepDuration = duration;
}

void TraceArguments::setMidSleepDuration(uint32_t duration) {
  m_MidSleepDuration = duration;
}

void TraceArguments::setApps(const vector<string> & apps) {
  m_Apps = apps;
}

void TraceArguments::setAndroidCategories(const vector<string> & categories) {
  m_AndroidCategories = categories;
}

void TraceArguments::setKernelCategories(const vector<string> & categories) {
  m_KernelCategories = categories;
}

void TraceArguments::setKernelFunctions(const vector<string> & functions) {
  m_KernelFunctions = functions;
}

bool TraceArguments::hasHelpMessage() const {
  return !m_HelpMessage.empty();
}
bool TraceArguments::hasKernelCategoryFilename() const {
  return !m_KernelCategoryFilename.empty();
}
bool TraceArguments::hasOutputFilename() const {
  return !m_OutputFilename.empty();
}
bool TraceArguments::hasApps() const {
  return !m_Apps.empty();
}
bool TraceArguments::hasAndroidCategories() const {
  return !m_AndroidCategories.empty();
}
bool TraceArguments::hasKernelCategories() const {
  return !m_KernelCategories.empty();
}
bool TraceArguments::hasKernelFunctions() const {
  return !m_KernelFunctions.empty();
}

bool TraceArguments::hasInitSleepDuration() const {
  return m_InitSleepDuration != 0;
}

bool TraceArguments::hasMidSleepDuration() const {
  return m_MidSleepDuration != 0;
}

bool TraceArguments::circleBufferEnabled() const {
  return m_CircleBuffer;
}

bool TraceArguments::ignoreSignalsEnabled() const {
  return m_IgnoreSignals;
}

bool TraceArguments::compressionEnabled() const {
  return m_Compression;
}

bool TraceArguments::asyncStartEnabled() const {
  return m_AsyncStart;
}

bool TraceArguments::asyncStopEnabled() const {
  return m_AsyncStop;
}

bool TraceArguments::asyncDumpEnabled() const {
  return m_AsyncDump;

}
bool TraceArguments::streamEnabled() const {
  return m_Stream;
}

bool TraceArguments::listCategoriesEnabled() const {
  return m_ListCategories;
}

bool TraceArguments::coreServicesEnabled() const {
  return m_CoreServices;
}

const string & TraceArguments::getKernelCategoryFilename() const {
  return m_KernelCategoryFilename;
}

const string & TraceArguments::getOutputFilename() const {
  return m_OutputFilename;
}

const string & TraceArguments::getAppName() const {
  return m_AppName;
}

uint32_t TraceArguments::getBufferSize() const {
  return m_BufferSize;
}

uint32_t TraceArguments::getInitSleepDuration() const {
  return m_InitSleepDuration;
}

uint32_t TraceArguments::getMidSleepDuration() const {
  return m_MidSleepDuration;
}

const vector<string> TraceArguments::getApps() const {
  return m_Apps;
}

const vector<string> & TraceArguments::getAndroidCategories() const {
  return m_AndroidCategories;
}

const vector<string> & TraceArguments::getKernelCategories() const {
  return m_KernelCategories;
}

const vector<string> & TraceArguments::getKernelFunctions() const {
  return m_KernelFunctions;
}

const string & TraceArguments::getHelpMessage() const {
  return m_HelpMessage;
}

TraceArguments::TraceArguments() {
}
