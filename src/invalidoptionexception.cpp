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

#include "invalidoptionexception.h"

#include <sstream>
#include <string>

InvalidOptionException::InvalidOptionException(const std::string & what_option):
    std::exception() {
  std::ostringstream oss;
  oss << "Invalid option: " << what_option;
  this->m_Message = oss.str();
}

InvalidOptionException::InvalidOptionException(const char * what_option):
    std::exception() {
  std::ostringstream oss;
  oss << "Invalid option: " << what_option;
  this->m_Message = oss.str();
}

const char* InvalidOptionException::what() const noexcept
{
  return this->m_Message.c_str();
}