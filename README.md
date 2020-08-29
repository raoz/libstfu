# libstfu

This is a small LD\_PRELOAD library that will direct stdin, stdout and stderr to /dev/null for specified executables by name.

## Usage

Example usage (assuming libstfu.so is in LD\_LIBRARY\_PATH, otherwise must give a path to libstfu.so):
```
STFU_EXE=someloudprogram LD_PRELOAD=libstfu.so someprogramthatcallssomeloudprogram
```

Multiple targets can be specified by separating with colons:
```
STFU_EXE=someloudprogram:anotherloudprogram LD_PRELOAD=libstfu.so someprogramthatcallsthem
```

## Contributing
Pull requests are welcome.

## License
Libstfu is licensed under the 3-Clause BSD License.
