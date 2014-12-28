// Copyright (c) 2015, pappes. All rights reserved. Use of this source code
// is governed by a BSD-style license that can be found in the LICENSE file.

library pappesUtility_test;

import 'package:playpen/pappesUtility.dart';
import 'package:unittest/unittest.dart';

void main() => defineTests();

void defineTests() {
  group('pappesUtility RamCache posative testing', () {
    test('integer retrieval', () {
      RamCache.remember('int', 1);
      RamCache.remember('float', 1.0);
      expect(RamCache.recall('float'), 1.0);
      RamCache.forget('float');
      expect(RamCache.recall('int'), 1);
    });
    test('session separation', () {
      RamCache.remember('int', 2, 'session2');
      RamCache.remember('int', 3, 'session3');
      RamCache.remember('int', 4, 'session4');
      expect(RamCache.recall('int', 'session4'), 4);
      RamCache.destroySessionCache('session4');
      expect(RamCache.recall('int', 'session2'), 2);
      expect(RamCache.recall('int', 'session3'), 3);
    });
    test('string retrieval', () {
      RamCache.remember('string1', 'abc');
      expect(RamCache.recall('string1'), 'abc');
    });
    test('forget single key', () {
      expect(RamCache.recall('float'), null);
    });
    test('forget session', () {
      expect(RamCache.recall('int', 'session4'), null);
    });
  });
  group('pappesUtility RamCache negative testing', () {
    test('absent value', () {
      expect(RamCache.recall('uninitialised key') == null, true);
    });
    test('dart language quirk - unrequested duplication of an object', () {
      RamCache.remember('number 10', 10);
      int tempInt = RamCache.recall('number 10');
      RamCache.remember('number 11', ++tempInt);
      expect(RamCache.recall('number 10'), 10);
      expect(RamCache.recall('number 11'), 11);
      
      RamCache.remember('string 1', 'hello');
      String tempStr = (RamCache.recall('string 1') as String).toUpperCase() ;
      RamCache.remember('string 2', tempStr);
      expect(RamCache.recall('string 1'), 'hello');
      expect(RamCache.recall('string 2'), 'HELLO');
    });
  });
}
