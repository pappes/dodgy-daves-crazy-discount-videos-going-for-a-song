// Copyright (c) 2014, <your name>. All rights reserved. Use of this source code
// is governed by a BSD-style license that can be found in the LICENSE file.

/// The playpen library.
library playpen;

int calculate() {
  return 6 * 7;
}

// Define a function.
printNumber(aNumber) => print('The number is $aNumber.'); // Print to console.


returnNumbers(int number1, [int number2= null, int number3= null, int number4= null, int number5= null]) {
  return 'The numbers are $number1' +
        (number2!=null ? ' ,$number2' : '') +
        (number3!=null ? ' ,$number3' : '')  + 
        (number4!=null ? ' ,$number4' : '')  + 
        (number5!=null ? ' ,$number5' : '')   +
        '.';
}

processNumbers(process, int number1, [int number2= null, int number3= null, int number4= null, int number5= null]) {
  process(returnNumbers( number1,  number2, number3, number4, number5));
}

testSwitch(){
  var command = 'CLOSED';
  switch (command) {
    case 'CLOSED':
      print('Closed');
      continue nowClosed; // Continues executing at the nowClosed label.

  nowClosed:
    case 'NOW_CLOSED':
      // Runs for both CLOSED and NOW_CLOSED.
      print('nowClosed');
      break;
  }
}

// Define a function.
List giveMeAList() {
  return [1, 2, 3, 4, 5, 6, 7];
}