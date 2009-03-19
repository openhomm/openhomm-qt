#!/usr/bin/env python
"""
History:
"""

""" TODO:
1. outdir ends on trailing slash? if no then add it
"""
import zlib, struct, os, sys, glob, fnmatch, profile

from optparse import OptionParser

class InvalidLod(Exception):
  pass

class Lod():
  __header = { 'id' : '', 'files' : 0 }
  __fat = {}

  def __init__(self):
    pass

  def __read_header(self):
    self.__header['id'] = self.__lod.read(8)
    
    if self.__header['id'] != "LOD\x00\xC8\x00\x00\x00" \
      and self.__header['id'] != 'LOD\x00\xF4\x01\x00\x00':
      raise InvalidLod
    
    self.__header['files'] = struct.unpack('i', self.__lod.read(4))
    self.__header['files'] = self.__header['files'][0]    # fuck off tuple
    self.__lod.seek(0x5C) # go to FAT

  def read_fat_record(self):
    name = self.__lod.read(12)
    name = name.split('\x00')[0]

    self.__lod.seek( self.__lod.tell() + 4 )              # skip some junk

    offset = struct.unpack( 'i', self.__lod.read( 4 ) )
    realsize = struct.unpack( 'i', self.__lod.read( 4 ) )

    self.__lod.seek( self.__lod.tell() + 4 )              # skip some junk

    size = struct.unpack('i', self.__lod.read(4))
    fat_record = ( offset, realsize, size)
    
    return name, fat_record

  def _lowercase_cmp(self, left, right):
    return cmp (left.lower(), right.lower())

  def open(self, filename):
    self.__lod = file(filename, "rb")
    self.__read_header()
    
  def fill_fat(self):
    for i in range(self.__header['files']):
      name, record = self.read_fat_record()
      self.__fat[name] = record

  def close(self):
    self.__lod.close()
    
  def list(self):
    self.fill_fat()
    keys = self.__fat.keys()
    keys.sort(self._lowercase_cmp)
    for key in keys:
      print "File: %s Offset: %d Realsize: %d Packsize: %d" % (
        key, self.__fat[key][0][0], self.__fat[key][1][0],
        self.__fat[key][2][0])
    
  def extract_files(self, pattern):
    self.fill_fat()
    keys = self.__fat.keys()
    keys = fnmatch.filter(keys, pattern)
    keys.sort(self._lowercase_cmp)

    for key in keys:
      self.__lod.seek(self.__fat[key][0][0])
      
      if self.__fat[key][2][0] > 0:
        try:
          readb = self.__lod.read(self.__fat[key][2][0])
          readb = zlib.decompress(readb)
        except e:
          print e
      else:
        readb = self.__lod.read(self.__fat[key][1][0])

      newfile = file(options_dict['outdir'] + key, "wb")
      newfile.write(readb)
      newfile.close()

if __name__ == "__main__":
  parser = OptionParser(usage="%prog [options] archive_name", version="%prog 1.0 prealpha2")
  addfiles = []

  parser.add_option("-a", "--add",
                    dest="addfiles", metavar="FILE",
                    help="Add files to archive")
  parser.add_option("-d", "--delete",
                    dest="delfiles", metavar="FILE",
                    help="Delete files from archive.")
  parser.add_option("-e", "--extract",
                    dest="extfiles", metavar="FILE",
                    help="Extract files to target directory.")
  parser.add_option("-l", "--list",
                    action="store_true", dest="listfiles",
                    help="List the contents of archive.")
                    
  parser.add_option("--cl", 
                    action="store_true", dest="lower",
                    help="Convert file names to lower case.")
  parser.add_option("--cu", 
                    action="store_true", dest="upper",
                    help="Convert file names to upper case.")

  parser.add_option("--out", dest="outdir", metavar="DIR",
                    help="Destination dir for unpack.")
                    
  parser.add_option("-q", "--quiet",
                    action="store_false", dest="verbose", default=True,
                    help="don't print status messages to stdout")

  (options, args) = parser.parse_args()

  if len(args) == 0:
    parser.print_help()
    
  options_dict = options.__dict__

  if options_dict['listfiles'] != None:
    loda = Lod()
    loda.open(args[0])
    loda.list()
    loda.close()

  if options_dict['lower'] != None and options_dict['upper'] != None:
    parser.error("options --cl and --cu are mutually exclusive")

  if options_dict['lower'] != None:
    pass
  if options_dict['upper'] != None:
    pass

  if options_dict['extfiles'] != None:
    loda = Lod()
    loda.open(args[0])
    #profile.run("loda.extract_files(options_dict['extfiles'])")
    loda.extract_files(options_dict['extfiles'])
    loda.close()

  if options_dict['addfiles'] or options_dict['delfiles']:
    if options_dict['addfiles']:
      addfiles = glob.glob(options.addfiles)
      print addfiles
    if options_dict['delfiles']:
      pass
