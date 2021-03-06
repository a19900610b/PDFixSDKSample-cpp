////////////////////////////////////////////////////////////////////////////////////////////////////
// RegisterEvent.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage RegisterEvent_cpp
*/
/*!
\page RegisterEvent_cpp Register Event Sample
// Example how to register events/callbacks.
\snippet /RegisterEvent.hpp RegisterEvent_cpp
*/

#pragma once

//! [RegisterEvent_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

// DocDidOpenCallback gets title when the document is opened.
void DocDidOpenCallback(PdfEventParams* event, void* data) {
  if (event->type != kEventDocDidOpen)
    throw std::runtime_error("This should never happen!");
  if (event->doc == nullptr)
    throw std::runtime_error("This should never happen!");
  std::cout << "Document was opened!" << std::endl;
  std::wstring title;
  title.resize(event->doc->GetInfo(L"Title", nullptr, 0));
  event->doc->GetInfo(L"Title", (wchar_t*)title.c_str(), title.size());
  std::wcout << title << std::endl;
}

// DocWillCallback notifies you when the event happens.
void DocWillCallback(PdfEventParams* event, void* data) {
  switch (event->type) {
  case kEventDocWillClose:
    std::cout << "Document will be closed!" << std::endl;
    break;
  case kEventDocWillSave:
    std::cout << "Document will be saved!" << std::endl;
    break;
  }
}

// Registers different kinds of events.
void RegisterEvent(
  const std::wstring& email,                     // authorization email   
  const std::wstring& license_key,               // authorization license key
  const std::wstring& open_path                  // source PDF document
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(pdfix->GetError());

  // add events
  pdfix->RegisterEvent(kEventDocDidOpen, &DocDidOpenCallback, nullptr);
  pdfix->RegisterEvent(kEventDocWillClose, &DocWillCallback, nullptr);
  pdfix->RegisterEvent(kEventDocWillSave, &DocWillCallback, nullptr);

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(pdfix->GetError());
  doc->Close();
  pdfix->Destroy();
}
//! [RegisterEvent_cpp]