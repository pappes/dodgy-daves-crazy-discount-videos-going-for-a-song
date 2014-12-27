// Copyright (c) 2014, <your name>. All rights reserved. Use of this source code
// is governed by a BSD-style license that can be found in the LICENSE file.

library all_tests;

import 'playpen_test.dart' as playpen_test;
import 'playpenUtility_test.dart' as playpenUtility_test;

void main() {
  playpen_test.defineTests();
  playpenUtility_test.defineTests();
}
