// Copyright (c) 2014, <your name>. All rights reserved. Use of this source code
// is governed by a BSD-style license that can be found in the LICENSE file.

/// The playpen library.
library playpenUtility;


class PlaypenUtility extends Object {
  //implimented as a singleton as all instances would behave the same anyway
  
  // Testing lazy initialisation of final variable
  static int hhgttg;

  static int calculate() {
    if (hhgttg == null) {
      hhgttg = 6 * 7;
    }
    return hhgttg;
  }


  factory PlaypenUtility() {
    //constructor as invoked by external instanciations
    return _singleton;
  }
  void _initialise() {
    //constructor as invoked by internal first creation    
  }
  
  static final _singleton = new PlaypenUtility()._initialise;
}
