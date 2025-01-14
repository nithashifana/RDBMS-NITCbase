#include "Buffer/StaticBuffer.h"
#include "Cache/OpenRelTable.h"
#include "Disk_Class/Disk.h"
#include "FrontendInterface/FrontendInterface.h"
#include <iostream>
#include <cstring>

// STAGE 1 Exercise printBlockAllocationMap
void S1Exercise(unsigned char *buffer)
{
  Disk::readBlock(buffer, 0);
  for (int i = 0; i < 9; i++)
  {
    printf("%d\n", (int)buffer[i]);
    // std::cout << i << " th position has value " << (int)buffer[i] << std::endl;
  }
}

void stage1()
{
  unsigned char buffer[BLOCK_SIZE];
  Disk::readBlock(buffer, 7000); // reading
  char message[] = "hello";
  memcpy(buffer + 20, message, 6); // copying
  Disk::writeBlock(buffer, 7000);  // writing

  // checking
  unsigned char buffer2[BLOCK_SIZE];
  Disk::readBlock(buffer2, 7000);
  char message2[6];
  memcpy(message2, buffer2 + 20, 6);
  printf("%s\n", message2);
}

void stage2()
{

  // create objects for the relation catalog and attribute catalog
  RecBuffer relCatBuffer(RELCAT_BLOCK);
  RecBuffer attrCatBuffer(ATTRCAT_BLOCK);

  HeadInfo relCatHeader;
  HeadInfo attrCatHeader;

  // load the headers of both the blocks into relCatHeader and attrCatHeader.
  // (we will implement these functions later)
  relCatBuffer.getHeader(&relCatHeader);
  attrCatBuffer.getHeader(&attrCatHeader);

  for (int i = 0; i < relCatHeader.numEntries; i++ /* i = 0 to total relation count */)
  {

    Attribute relCatRecord[RELCAT_NO_ATTRS]; // will store the record from the relation catalog

    relCatBuffer.getRecord(relCatRecord, i);

    printf("Relation: %s\n", relCatRecord[RELCAT_REL_NAME_INDEX].sVal);

    int headerNum = ATTRCAT_BLOCK;
    while (headerNum != -1)
    {
      RecBuffer attrCatBuffer(headerNum);
      attrCatBuffer.getHeader(&attrCatHeader);

      for (int j = 0; j < attrCatHeader.numEntries; j++ /* j = 0 to number of entries in the attribute catalog */)
      {

        Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
        attrCatBuffer.getRecord(attrCatRecord, j);

        // declare attrCatRecord and load the attribute catalog entry into it

        if (strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal, relCatRecord[RELCAT_REL_NAME_INDEX].sVal) == 0 /* attribute catalog entry corresponds to the current relation */)
        {
          const char *attrType = attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER ? "NUM" : "STR";
          printf("  %s: %s\n", attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal /* get the attribute name */, attrType);
        }
      }
      headerNum = attrCatHeader.rblock;
    }
    printf("\n");
  }
}

// STAGE 2 Excercise
void S2Exercise()
{
}

void stage3()
{
  // Note: relId is 0,1 for rel, attr tables respectively
  //(i.e RELCAT_RELID and ATTRCAT_RELID)

  for (int relId = 0; relId < 2; relId++)
  {
    RelCatEntry RelCatBuf;
    RelCacheTable::getRelCatEntry(relId, &RelCatBuf);

    // get the relation catalog entry using RelCacheTable::getRelCatEntry()
    printf("Relation: %s\n", RelCatBuf.relName);

    for (int j = 0; j < RelCatBuf.numAttrs; j++)
    {
      AttrCatEntry AttrCatBuf;
      AttrCacheTable::getAttrCatEntry(relId, j, &AttrCatBuf);
      // get the attribute catalog entry for (rel-id i, attribute offset j)
      //  in attrCatEntry using AttrCacheTable::getAttrCatEntry()

      printf("  %s: ", AttrCatBuf.attrName);
      if(AttrCatBuf.attrType == 0) {
        printf("NUM\n");
      }
      else printf("STR\n");
    }
    printf("\n");
  }
}

int main(int argc, char *argv[])
{
  /* Initialize the Run Copy of Disk */
  Disk disk_run;
  // stage1();
  //  S1Exercise (buffer);

  // stage2();
  StaticBuffer buffer;
  OpenRelTable cache;
  stage3();
  // return FrontendInterface::handleFrontend(argc, argv);
  // return 0;
}
