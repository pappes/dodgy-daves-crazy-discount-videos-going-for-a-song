// Copyright (c) 2014, <your name>. All rights reserved. Use of this source code
// is governed by a BSD-style license that can be found in the LICENSE file.

library playpenUtility_test;

import 'package:playpen/playpenUtility.dart';
import 'package:unittest/unittest.dart';

void main() => defineTests();

void defineTests() {
  /**/group('singlton tests', () {
    final PlaypenUtility ppu1 = new PlaypenUtility();
    final PlaypenUtility ppu2 = new PlaypenUtility();
    test('same hash h', () {
      expect(ppu1.hashCode == ppu2.hashCode, true);
    });
    test('same hash', () {
      expect(new PlaypenUtility().hashCode == new PlaypenUtility().hashCode, true);
    });
    test('equals', () {
      expect(new PlaypenUtility() == new PlaypenUtility(), true);
    });
    test('identical', () {
      expect(identical(new PlaypenUtility(), new PlaypenUtility()), true);
    });
  });/**/
}
