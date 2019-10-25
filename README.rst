=====
petro
=====

.. image:: https://travis-ci.org/steinwurf/petro.svg?branch=master
    :target: https://travis-ci.org/steinwurf/petro

.. image:: ./petro.svg
   :height: 200px
   :width: 200px

petro is an mp4 parser which allows you to extract all kinds of information.

The implementation is based on `ISO/IEC 14496-12 <http://www.iso.org/iso/catalogue_detail.htm?csnumber=61988>`_,
link to the `2015 revision <http://standards.iso.org/ittf/PubliclyAvailableStandards/c068960_ISO_IEC_14496-12_2015.zip>`_

Usage
=====

.. image:: ./petro.gif

More examples of how to use petro is located in the examples folder.

Building shared library
-----------------------

Configure with -fPIC to enable building a shared lib::

    ./waf configure ... --cxxflags=-fPIC

The `-fPIC` flag is needed to build a shared library read more about that here:
http://stackoverflow.com/a/19768349

Use as Dependency in CMake
--------------------------

To depend on this project when using the CMake build system, add the following
in your CMake build script:

::

   add_subdirectory("/path/to/petro" petro)
   target_link_libraries(<my_target> steinwurf::petro)

Where ``<my_target>`` is replaced by your target.
