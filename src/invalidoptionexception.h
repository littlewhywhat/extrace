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

#ifndef LTTLWHWHT_INVARGEXCEPT_H
#define LTTLWHWHT_INVARGEXCEPT_H

#include <string>
#include <stdexcept>

class InvalidOptionException : public std::exception
{
  public:
    InvalidOptionException(const std::string & what_option);
    InvalidOptionException(const char * what_option);
    const char* what() const noexcept override;
  private:
    std::string m_Message;
};

#endif // LTTLWHWHT_INVARGEXCEPT_H