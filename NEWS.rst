News for petro
==============

This file lists the major changes between versions. For a more detailed list of
every change, see the Git log.

Latest
------
* tbd
* Minor: Added templated version of ``box::get_children``.
* Minor: Added ``data_box::data()``.
* Minor: Added ``extractor`` abstract class.
* Major: Changed ``extractor`` ``open`` from ``open(error)`` to
  ``open(data, size, track_id, error)``.
* Major: Changed extractor to now also need the track id of the track they
  should extract. Use the newly added ``track_extractor`` to get appropriate
  id.
* Minor: Added looping API to ``extractor``.
* Minor: Added ``write_nalu_header`` and ``nalu_header_size`` to
  ``write_nalu_header``.
* Major: Removed ``annex_b_writer``, ``nalu_extractor`` and
  ``avc_sample_access_layer`` ``avc_to_annex_b`` and addition to the
  ``avc_sample_extractor`` should be used instead.
* Major: Replaced ``memory_mapped_file_layer`` with ``extractor::file`` and
  ``data_layer``.
* Major: Loop layer no disabled by default.

14.0.1
------
* Patch: Fix bug where opening an extractor with a none existing file would
  cause an exception to be thrown.

14.0.0
------
* Major: Upgrade to endian 10.

13.0.0
------
* Major: Upgrade to endian 9.

12.0.0
------
* Major: Correct spelling of descriptor/tag enums:
  ``elemetary_stream`` to ``elementary_stream``, and
  ``ip_dentification_data_set`` to ``ip_identification_data_set``.
* Patch: Add virtual destructors to base classes.

11.0.0
------
* Major: Upgrade to endian 8.

10.0.0
------
* Major: Upgrade to endian 6.

9.1.0
-----
* Minor: Added `samples` function to extractors to get the number of samples.

9.0.0
-----
* Major: Changed API of ``sequence_parameter_set`` and
  ``picture_parameter_set``. The constructor has been made private, and the
  object can now only be created using the static ``parse`` function.

8.0.1
-----
* Patch: Set the ``install_path`` of the petro shared lib to ``None`` to avoid
  automatic installation to system directories during the waf install step.

8.0.0
-----
* Major: Added error codes.
* Major: ``bit_reader`` renamed to ``bit_stream``.
* Major: ``byte_stream`` was replaced by a wrapper of an ``endian::reader``.
* Major: The ``extractor``\ s open function now takes an error code and doesn't
  rely on the returned boolean to announce the occurrence of an error.
* Major: ``sequence_parameter_set`` and ``picture_parameter_set`` now needs to
  be parsed.
* Major: Changed constructor parameters of ``box``.
* Major: Renamed ``data_box::read`` to ``data_box::parse``.
* Major: Split ``box`` into two classes; ``box::box`` and ``box::data_box``.

7.2.0
-----
* Minor: Moved ``avc_sample_access`` to it's own layer called
  ``avc_sample_access_layer``.

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
* Minor: Added the ``media_duration`` function in
  ``timestamp_extractor_layer``.
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
* Minor: Added free function to calculate the presentation time in
  microseconds.
* Minor: Extended ctts and stts API to easier extract compressed data.

1.2.1
-----
* Patch: Added assert which checks if the given file exists.

1.2.0
-----
* Minor: Added templated member function for getting children of a certain
  type.

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
