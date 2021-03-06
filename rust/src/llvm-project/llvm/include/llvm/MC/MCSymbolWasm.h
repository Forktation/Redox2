//===- MCSymbolWasm.h -  ----------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_MC_MCSYMBOLWASM_H
#define LLVM_MC_MCSYMBOLWASM_H

#include "llvm/BinaryFormat/Wasm.h"
#include "llvm/MC/MCSymbol.h"

namespace llvm {

class MCSymbolWasm : public MCSymbol {
  wasm::WasmSymbolType Type = wasm::WASM_SYMBOL_TYPE_DATA;
  bool IsWeak = false;
  bool IsHidden = false;
  bool IsComdat = false;
  mutable bool IsUsedInInitArray = false;
  mutable bool IsUsedInGOT = false;
  Optional<std::string> ImportModule;
  Optional<std::string> ImportName;
  Optional<std::string> ExportName;
  wasm::WasmSignature *Signature = nullptr;
  Optional<wasm::WasmGlobalType> GlobalType;
  Optional<wasm::WasmEventType> EventType;

  /// An expression describing how to calculate the size of a symbol. If a
  /// symbol has no size this field will be NULL.
  const MCExpr *SymbolSize = nullptr;

public:
  // Use a module name of "env" for now, for compatibility with existing tools.
  // This is temporary, and may change, as the ABI is not yet stable.
  MCSymbolWasm(const StringMapEntry<bool> *Name, bool isTemporary)
      : MCSymbol(SymbolKindWasm, Name, isTemporary) {}
  static bool classof(const MCSymbol *S) { return S->isWasm(); }

  const MCExpr *getSize() const { return SymbolSize; }
  void setSize(const MCExpr *SS) { SymbolSize = SS; }

  bool isFunction() const { return Type == wasm::WASM_SYMBOL_TYPE_FUNCTION; }
  bool isData() const { return Type == wasm::WASM_SYMBOL_TYPE_DATA; }
  bool isGlobal() const { return Type == wasm::WASM_SYMBOL_TYPE_GLOBAL; }
  bool isSection() const { return Type == wasm::WASM_SYMBOL_TYPE_SECTION; }
  bool isEvent() const { return Type == wasm::WASM_SYMBOL_TYPE_EVENT; }
  wasm::WasmSymbolType getType() const { return Type; }
  void setType(wasm::WasmSymbolType type) { Type = type; }

  bool isExported() const {
    return getFlags() & wasm::WASM_SYMBOL_EXPORTED;
  }
  void setExported() const {
    modifyFlags(wasm::WASM_SYMBOL_EXPORTED, wasm::WASM_SYMBOL_EXPORTED);
  }

  bool isNoStrip() const {
    return getFlags() & wasm::WASM_SYMBOL_NO_STRIP;
  }
  void setNoStrip() const {
    modifyFlags(wasm::WASM_SYMBOL_NO_STRIP, wasm::WASM_SYMBOL_NO_STRIP);
  }

  bool isWeak() const { return IsWeak; }
  void setWeak(bool isWeak) { IsWeak = isWeak; }

  bool isHidden() const { return IsHidden; }
  void setHidden(bool isHidden) { IsHidden = isHidden; }

  bool isComdat() const { return IsComdat; }
  void setComdat(bool isComdat) { IsComdat = isComdat; }

  const StringRef getImportModule() const {
      if (ImportModule.hasValue()) {
          return ImportModule.getValue();
      }
      return "env";
  }
  void setImportModule(StringRef Name) { ImportModule = Name; }

  bool hasImportName() const { return ImportName.hasValue(); }
  const StringRef getImportName() const {
      if (ImportName.hasValue()) {
          return ImportName.getValue();
      }
      return getName();
  }
  void setImportName(StringRef Name) { ImportName = Name; }

  bool hasExportName() const { return ExportName.hasValue(); }
  const StringRef getExportName() const { return ExportName.getValue(); }
  void setExportName(StringRef Name) { ExportName = Name; }

  void setUsedInGOT() const { IsUsedInGOT = true; }
  bool isUsedInGOT() const { return IsUsedInGOT; }

  void setUsedInInitArray() const { IsUsedInInitArray = true; }
  bool isUsedInInitArray() const { return IsUsedInInitArray; }

  const wasm::WasmSignature *getSignature() const { return Signature; }
  void setSignature(wasm::WasmSignature *Sig) { Signature = Sig; }

  const wasm::WasmGlobalType &getGlobalType() const {
    assert(GlobalType.hasValue());
    return GlobalType.getValue();
  }
  void setGlobalType(wasm::WasmGlobalType GT) { GlobalType = GT; }

  const wasm::WasmEventType &getEventType() const {
    assert(EventType.hasValue());
    return EventType.getValue();
  }
  void setEventType(wasm::WasmEventType ET) { EventType = ET; }
};

} // end namespace llvm

#endif // LLVM_MC_MCSYMBOLWASM_H
