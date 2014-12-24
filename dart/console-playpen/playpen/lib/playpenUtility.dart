// Copyright (c) 2014, <your name>. All rights reserved. Use of this source code
// is governed by a BSD-style license that can be found in the LICENSE file.

/// The playpen library.
library playpenUtility;


class PlaypenUtility extends Object {
  // Testing lazy initialisation of final variable
  static int hhgttg;

  static int calculate() {
    if (hhgttg == null) {
      hhgttg = 6 * 7;
    }
    return hhgttg;
  }
}
/*
void useVMConfiguration() {
  unittestConfiguration = _singleton;
}*/
/*
final _singleton = new PlaypenUtility();*/
