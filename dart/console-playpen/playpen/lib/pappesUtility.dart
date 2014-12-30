// Copyright (c) 2015, pappes. All rights reserved. Use of this source code
// is governed by a BSD-style license that can be found in the LICENSE file.

/// Collection of utility classes and functions to make programs simpler.
library pappesUtility;


part 'pappesUtility_RamCache.dart';




/// Substitutes the value [alternate] if the value [test] is null.
/// For exampple:
/// 
///    x = ifNull(x, y);//use x if it has a value otherwise use y
String ifNull(test, alternate) {
  return test != null ? test : alternate;
}