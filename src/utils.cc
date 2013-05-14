/*
 * Copyright (c) 2013 LinkedIn Corp. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of the license at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the
 * License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.
 *
 */

/**
 * @file utils.cc
 * @author Brian Geffon
 * @author Manjesh Nilange
 */

#include "atscppapi/utils.h"
#include <sstream>
#include <arpa/inet.h>
#include <ts/ts.h>
#include "logging_internal.h"

const std::string atscppapi::utils::DISABLE_DATA_CACHING_ENV_FLAG("ATSCPPAPI_DISABLE_TRANSACTION_DATA_CACHING");

std::string atscppapi::utils::getIpString(const sockaddr *sockaddress) {
  if (sockaddress == NULL) {
    LOG_ERROR("Cannot work on NULL sockaddress");
   return std::string();
  }

  char buf[INET6_ADDRSTRLEN];

  switch (sockaddress->sa_family) {
  case AF_INET:
    inet_ntop(AF_INET, &(((struct sockaddr_in *) sockaddress)->sin_addr), buf, INET_ADDRSTRLEN);
    return std::string(buf);
  case AF_INET6:
    inet_ntop(AF_INET6, &(((struct sockaddr_in6 *) sockaddress)->sin6_addr), buf, INET6_ADDRSTRLEN);
    return std::string(buf);
  default:
    LOG_ERROR("Unknown Address Family %d", static_cast<int>(sockaddress->sa_family));
    return std::string();
  }
}

uint16_t atscppapi::utils::getPort(const sockaddr *sockaddress) {
  if (sockaddress == NULL) {
    LOG_ERROR("Cannot work on NULL sockaddress");
    return 0;
  }

  if (sockaddress->sa_family == AF_INET) {
    return ntohs((((struct sockaddr_in*) sockaddress)->sin_port));
  } else if (sockaddress->sa_family == AF_INET6) {
    return ntohs((((struct sockaddr_in6*) sockaddress)->sin6_port));
  } else {
    LOG_ERROR("Unknown Address Family %d", static_cast<int>(sockaddress->sa_family));
    return 0;
  }
}

std::string atscppapi::utils::getIpPortString(const sockaddr *sockaddress) {
  if (sockaddress == NULL) {
    LOG_ERROR("Cannot work on NULL sockaddress");
    return std::string();
  }

  std::ostringstream oss;
  oss << getIpString(sockaddress);
  oss << ":";
  oss << getPort(sockaddress);
  return oss.str();
}
