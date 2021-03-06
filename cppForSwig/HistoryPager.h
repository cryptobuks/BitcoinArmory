#ifndef HISTORY_PAGER_H
#define HISTORY_PAGER_H

#include <map>
#include <functional>

#include "BinaryData.h"
#include "LedgerEntry.h"
#include "BlockObj.h"

class HistoryPager
{
private:

   struct Page
   {
      uint32_t blockStart_;
      uint32_t blockEnd_;
      uint32_t count_;

      map<BinaryData, LedgerEntry> pageLedgers_;

      Page(void) : blockStart_(UINT32_MAX), blockEnd_(UINT32_MAX), count_(0)
      {}

      Page(uint32_t count, uint32_t bottom, uint32_t top) :
         blockStart_(bottom), blockEnd_(top), count_(count)
      {}

      bool operator< (const Page& rhs) const
      {
         //history pages are order backwards
         return this->blockStart_ > rhs.blockStart_;
      }
   };

   vector<Page> pages_;
   map<uint32_t, uint32_t> SSHsummary_;

   uint32_t currentPage_ = 0;
   
   static uint32_t txnPerPage_;

public:

   HistoryPager(void) {}

   map<BinaryData, LedgerEntry>& getPageLedgerMap(
      function< void(uint32_t, uint32_t, map<BinaryData, TxIOPair>& ) > getTxio,
      function< void(map<BinaryData, LedgerEntry>&, 
                     const map<BinaryData, TxIOPair>&, uint32_t) > buildLedgers,
      uint32_t pageId,
      map<BinaryData, TxIOPair>* txioMap = nullptr);

   void getPageLedgerMap(
      function< void(uint32_t, uint32_t, map<BinaryData, TxIOPair>&) > getTxio,
      function< void(map<BinaryData, LedgerEntry>&,
      const map<BinaryData, TxIOPair>&, uint32_t, uint32_t) > buildLedgers,
      uint32_t pageId,
      map<BinaryData, LedgerEntry>& leMap) const;

   map<BinaryData, LedgerEntry>& getPageLedgerMap(uint32_t pageId);

   void reset(void) { pages_.clear(); }

   void addPage(uint32_t count, uint32_t bottom, uint32_t top);
   void sortPages(void) { std::sort(pages_.begin(), pages_.end()); }
   
   void mapHistory(
      function< map<uint32_t, uint32_t>(bool) > getSSHsummary,
      bool forcePaging = true);
   
   const map<uint32_t, uint32_t>& getSSHsummary(void) const
   { return SSHsummary_; }
   
   uint32_t getPageBottom(uint32_t id) const;
   size_t   getPageCount(void) const { return pages_.size(); }
   uint32_t getCurrentPage(void) const { return currentPage_; }
   void setCurrentPage(uint32_t pageId) { currentPage_ = pageId; }
   
   uint32_t getRangeForHeightAndCount(uint32_t height, uint32_t count) const;
};

#endif