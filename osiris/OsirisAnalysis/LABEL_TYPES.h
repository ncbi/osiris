/*
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE                          
*               National Center for Biotechnology Information
*                                                                          
*  This software/database is a "United States Government Work" under the   
*  terms of the United States Copyright Act.  It was written as part of    
*  the author's official duties as a United States Government employee and 
*  thus cannot be copyrighted.  This software/database is freely available 
*  to the public for use. The National Library of Medicine and the U.S.    
*  Government have not placed any restriction on its use or reproduction.  
*                                                                          
*  Although all reasonable efforts have been taken to ensure the accuracy  
*  and reliability of the software and data, the NLM and the U.S.          
*  Government do not and cannot warrant the performance or results that    
*  may be obtained by using this software or data. The NLM and the U.S.    
*  Government disclaim all warranties, express or implied, including       
*  warranties of performance, merchantability or fitness for any particular
*  purpose.                                                                
*                                                                          
*  Please cite the author in any work or product based on this material.   
*
* ===========================================================================
*
*  FileName: LABEL_TYPES.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __LABEL_TYPES_H__
#define __LABEL_TYPES_H__

//
// CComboLabels::GetChoices must be
// in a consistent order as LABEL_xxx_TYPE below
//
enum LABEL_PLOT_TYPE
{
  LABEL_NONE = 0,
  LABEL_ALLELE,
  LABEL_BPS,
  LABEL_RFU,
  LABEL_TIME,
  LABEL_PEAK_AREA,
  LABEL_PLOT_TYPE_COUNT
};

#define LABEL_PLOT_TYPE_MAX (LABEL_PLOT_TYPE_COUNT - 1)

enum LABEL_CELL_TYPE
{
  LABEL_CELL_ALLELE = 0,
  LABEL_CELL_BPS,
  LABEL_CELL_RFU,
  LABEL_CELL_TIME,
  LABEL_CELL_PEAK_AREA
};

static inline int PLOT_TO_CELL(int n)
{
  return (n > 0) ? (n - 1) : 0;
}
static inline int CELL_TO_PLOT(int n)
{
  return n + 1;
}


#endif
