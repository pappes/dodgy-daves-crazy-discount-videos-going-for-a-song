// Copyright (c) 2014, <your name>. All rights reserved. Use of this source code
// is governed by a BSD-style license that can be found in the LICENSE file.

/// The playpen library.
library playpenUtility;


class PlaypenUtility extends Object {
  //implimented as a singleton as all instances would behave the same anyway
  static final _singleton = new PlaypenUtility._initialise();

  // Testing lazy initialisation of final variable
  //note in DART all final variables are lazy loaded 
  //hence I could not declare this one as final
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
  PlaypenUtility._initialise() {
    //constructor as invoked by internal first creation
  }

}

//todo turn this into a RamCache memory factory 
//with remember, recall and forget methods 
//that can store arbitary data in memory
//and can opionally segment the memory per session for server instances
