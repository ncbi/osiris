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
*  FileName: CPrintParameters.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_PRINT_PARAMETERS_H__
#define __C_PRINT_PARAMETERS_H__

#include "wxIDS.h"

#define PRINT_LABEL_ARTIFACT_NONE ARTIFACT_NONE
#define PRINT_LABEL_ARTIFACT_ALL ARTIFACT_ALL
#define PRINT_LABEL_ARTIFACT_CRITICAL ARTIFACT_CRITICAL

#define PRINT_Y_SCALE_CHANNEL 0
#define PRINT_Y_SCALE_SAMPLE 1
#define PRINT_Y_SCALE_USER 2

#define PRINT_Y_SCALE_NEG_PEAKS 0
#define PRINT_Y_SCALE_NEG_INCLUDE_RFU 1
#define PRINT_Y_SCALE_NEG_ILS 2

#define PRINT_X_SCALE_NO_PRIMER_PEAK 0 
#define PRINT_X_SCALE_ALL_PRIMER_PEAK 1
#define PRINT_X_SCALE_CTRL_PRIMER_PEAK 2
#define PRINT_X_SCALE_USER 3

#define PRINT_TITLE_FILE_NAME 0
#define PRINT_TITLE_SAMPLE_NAME 1

#define PRINT_X_BPS 1
#define PRINT_X_TIME 0

#endif