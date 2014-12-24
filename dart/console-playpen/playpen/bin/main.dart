// Copyright (c) 2014, <your name>. All rights reserved. Use of this source code
// is governed by a BSD-style license that can be found in the LICENSE file.

import 'package:playpen/playpen.dart' as playpen;
import 'package:playpen/playpenUtility.dart' as playpenUtility;

main() {
  print('Hello world: ${playpen.calculate()}!');
  
  playpen.printNumber(playpenUtility.PlaypenUtility.calculate());
  

  var list = playpen.giveMeAList();
  list.forEach((row) => print(row)); 
  playpen.processNumbers(print,list[0],list[1],list[2]);
  
  var customfunc = print;
  playpen.processNumbers(customfunc,list[0],list[1],list[2]);
  var customfunc2 = (argus) => print('cf2 = ${argus}');
  playpen.processNumbers(customfunc2,list[0],list[1],list[2]);
  
  assert(playpen.returnNumbers(1,2,3) == 'The numbers are 1 ,2 ,3.');
}
