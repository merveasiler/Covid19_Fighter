#include "NucleoBase.h"

NucleoBase::NucleoBase(string stringChain, NucleoBase* prev) {
  this->prev = prev;
  this->data = stringChain[0];
  if (stringChain.length() == 1) {
    this->next = NULL;
    return;
  }
  this->next = new NucleoBase(stringChain.substr(1), this);
}

NucleoBase::NucleoBase(const NucleoBase& nucleobase) {
  this->data = nucleobase.data;
  // null previous nucleobase
  if (nucleobase.prev == NULL)
    this->prev = NULL;
  // next nucleobase
  if (nucleobase.next == NULL)
    this->next = NULL;
  else {
    this->next = new NucleoBase(*(nucleobase.next));  // recursive copy consturctor
    this->next->prev = this;                          // previous nucleobase of the next
  }
}

NucleoBase::~NucleoBase() {
  this->prev = NULL;
  if (this->next != NULL)
    delete this->next;    // recursive destructor
}

NucleoBase* NucleoBase::GetLast() {
  if (this->next == NULL)
    return this;
  return this->next->GetLast();
}

ostream& operator<<(ostream& os, const NucleoBase& nucleobase) {
  if (nucleobase.next != NULL)
    os << nucleobase.data << *nucleobase.next;  // recursive call
  else
    os << nucleobase.data;
  return os;
}
