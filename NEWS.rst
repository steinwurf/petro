News for petro
==============

This file lists the major changes between versions. For a more detailed list of
every change, see the Git log.

Latest
------
* tbd

7.1.0
-----
* Minor: Added missing functions to `aac_sample_extractor`.

7.0.0
-----
* Major: Upgrade to waf-tools 4
* Major: Upgrade to boost 3
* Minor: Upgrade to gtest 4
* Minor: Upgrade to stub 6
* Minor: Changed `aac_sample_extractor` to use the pimpl pattern.

6.0.0
-----
* Major: Removed the ambiguous ``timestamp`` function from extractors.
* Minor: Added the ``media_duration`` function in ``timestamp_extractor_layer``.
* Minor: Use the total media duration as a loop offset for decoding and
  presentation timestamps in ``looper_layer``.
* Minor: Changed `avc_sample_extractor` to use the pimpl pattern.
* Minor: Enabled building petro as a shared library

5.0.0
-----
* Major: Changed from using `std::ifstream` to using
  `boost::iostreams::mapped_file_source` for reading files.
* Major: Changed the order of the parameters given to `petro::parser::read` now
  the parent paramter is the last one.
* Major: Changed `petro::parser::read` to return a reference to the parent,
  and have a default value for the root paramter.
* Major: The extractors now have an open function which are allowed to fail,
  instead of doing stuff that can fail in the constructor.
* Major: Extractors are now split into seperate layers making it easier to test
  and reuse.
* Major: `bit_reader` now takes size in bytes instead of bits.
* Minor: Added `data_offset` function to byte_reader. Allowing a pointer to the
  current position of the byte_reader to be returned.
* Major: `picture_parameter_set` and `sequence_parameter_set` now uses shallow
  storage.
* Major: the `read_time32` and `read_time64` now returns a UTC time.
* Patch: the `read_time32` and `read_time64` now handles timestamps before
  1970-01-01 00:00:00.
* Major: Renamed `aac_extractor` -> `aac_sample_extractor`.
* Major: Renamed `h264_extractor` -> `avc_sample_extractor`.
* Minor: Added `annex_b_writer` for writing annex_b samples.
* Major: Added external dependency on boost.

4.0.0
-----
* Minor: Added the ``decoding_time`` helper function.
* Minor: Added the ``aac_extractor`` and ``h264_extractor`` classes to extract
  AAC and H.264 samples and the corresponding timestamps from mp4 files.
  The extractors provide simple iteration over the available samples.
* Major: The ``byte_stream`` constructor takes an istream reference instead
  of the ``filename`` as a string.

3.2.1
------
* Patch: Made ``presentation_time`` ``inline`` to prevent multiple definitions
  linker error.

3.2.0
------
* Minor: Added getters for the private fields of ``sl_config_descriptor``.
* Patch: Fixed warnings.
* Minor: Added license.

3.1.0
-----
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
