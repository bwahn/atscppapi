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

#include <iostream>
#include <atscppapi/GlobalPlugin.h>
#include <atscppapi/TransactionPlugin.h>
#include <atscppapi/PluginInit.h>

using namespace atscppapi;

class TransactionHookPlugin : public atscppapi::TransactionPlugin {
public:
  TransactionHookPlugin(Transaction &transaction) : TransactionPlugin(transaction) {
    char_ptr_ = new char[100];
    TransactionPlugin::registerHook(HOOK_SEND_RESPONSE_HEADERS);
    std::cout << "Constructed!" << std::endl;
  }
  virtual ~TransactionHookPlugin() {
    delete[] char_ptr_; // cleanup
    std::cout << "Destroyed!" << std::endl;
  }
  void handleSendResponseHeaders(Transaction &transaction) {
    std::cout << "Send response headers!" << std::endl;
    transaction.resume();
  }
private:
  char *char_ptr_;
};

class GlobalHookPlugin : public atscppapi::GlobalPlugin {
public:
  GlobalHookPlugin() {
    GlobalPlugin::registerHook(HOOK_READ_REQUEST_HEADERS_PRE_REMAP);
  }
  virtual void handleReadRequestHeadersPreRemap(Transaction &transaction) {
    std::cout << "Hello from handleReadRequesHeadersPreRemap!" << std::endl;
    transaction.addPlugin(new TransactionHookPlugin(transaction));
    transaction.resume();
  }
};

void TSPluginInit(int argc, const char *argv[]) {
  atscppapi::GlobalPlugin *instance = new GlobalHookPlugin();
}
