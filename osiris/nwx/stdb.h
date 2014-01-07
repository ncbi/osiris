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
*  FileName: stdb.h
*  Author:   Douglas Hoffman
*
*  in MS VC++ when compiling with an #include to a STL file, there are a lot of
*  warning in the form of 
*    warning C4702: unreachable code
*  the pragma below prevents that error
*  here is an example of usage
*
* #include "nwx/stdb.h"
* #include <vector>
* #include "nwx/stde.h"
*
*/

#ifdef __WXMSW__
#pragma warning (push)
#pragma warning (disable : 4702 )
#endif
