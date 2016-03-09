News for petro
==============

This file lists the major changes between versions. For a more detailed list of
every change, see the Git log.

Latest
------
* Minor: Expose `entry_count`, `entries`, and `chunk_offset` in `co64` box to
  allow extraction of media data in large mp4 files.

3.0.0
-----
* Major: Replaced read_N_bits API with read_bits(N) API.
* Minor: Added `sequence_parameter_set` and `picture_parameter_set`.
  Note no parsing is done in `picture_parameter_set`.

2.0.1
-----
* Patch: Added missing files.

2.0.0
-----
* Major: Removed redundant type string when getting specific child.
* Minor: Added free function to calculate the presentation time in microseconds.
* Minor: Extended ctts and stts API to easier extract compressed data.

1.2.1
-----
* Patch: Added assert which checks if the given file exists.

1.2.0
-----
* Minor: Added templated member function for getting children of a certain type.

1.1.0
-----
* Minor: Added more getters.

1.0.0
-----
* Major: Actual release.
* Minor: Added example for audio (AAC) extraction.

0.0.0
-----
* Major: Initial release.
