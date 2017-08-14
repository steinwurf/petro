// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

PETRO_ERROR_TAG(unused, "reserved since errors should never be zero.")
PETRO_ERROR_TAG(invalid_stream_read, "Invalid stream read.")
PETRO_ERROR_TAG(invalid_bit_stream_read, "Invalid bit stream read.")
PETRO_ERROR_TAG(invalid_box, "Unable to parse box.")
PETRO_ERROR_TAG(invalid_full_box, "Unable to parse full box.")
PETRO_ERROR_TAG(invalid_avcc_box, "Unable to read AVCC box")
PETRO_ERROR_TAG(invalid_bxml_box, "Unable to read BXML box")
PETRO_ERROR_TAG(invalid_co64_box, "Unable to read CO64 box")
PETRO_ERROR_TAG(invalid_cprt_box, "Unable to read CPRT box")
PETRO_ERROR_TAG(invalid_ctts_box, "Unable to read CTTS box")
PETRO_ERROR_TAG(invalid_dinf_box, "Unable to read DINF box")
PETRO_ERROR_TAG(invalid_dref_box, "Unable to read DREF box")
PETRO_ERROR_TAG(invalid_edts_box, "Unable to read EDTS box")
PETRO_ERROR_TAG(invalid_elst_box, "Unable to read ELST box")
PETRO_ERROR_TAG(invalid_esds_box, "Unable to read ESDS box")
PETRO_ERROR_TAG(invalid_free_box, "Unable to read FREE box")
PETRO_ERROR_TAG(invalid_frma_box, "Unable to read FRMA box")
PETRO_ERROR_TAG(invalid_ftyp_box, "Unable to read FTYP box")
PETRO_ERROR_TAG(invalid_hdlr_box, "Unable to read HDLR box")
PETRO_ERROR_TAG(invalid_hmhd_box, "Unable to read HMHD box")
PETRO_ERROR_TAG(invalid_iinf_box, "Unable to read IINF box")
PETRO_ERROR_TAG(invalid_iloc_box, "Unable to read ILOC box")
PETRO_ERROR_TAG(invalid_imif_box, "Unable to read IMIF box")
PETRO_ERROR_TAG(invalid_ipmc_box, "Unable to read IPMC box")
PETRO_ERROR_TAG(invalid_ipro_box, "Unable to read IPRO box")
PETRO_ERROR_TAG(invalid_mdat_box, "Unable to read MDAT box")
PETRO_ERROR_TAG(invalid_mdhd_box, "Unable to read MDHD box")
PETRO_ERROR_TAG(invalid_mdia_box, "Unable to read MDIA box")
PETRO_ERROR_TAG(invalid_mehd_box, "Unable to read MEHD box")
PETRO_ERROR_TAG(invalid_meta_box, "Unable to read META box")
PETRO_ERROR_TAG(invalid_mfhd_box, "Unable to read MFHD box")
PETRO_ERROR_TAG(invalid_mfra_box, "Unable to read MFRA box")
PETRO_ERROR_TAG(invalid_mfro_box, "Unable to read MFRO box")
PETRO_ERROR_TAG(invalid_minf_box, "Unable to read MINF box")
PETRO_ERROR_TAG(invalid_moof_box, "Unable to read MOOF box")
PETRO_ERROR_TAG(invalid_moov_box, "Unable to read MOOV box")
PETRO_ERROR_TAG(invalid_mvex_box, "Unable to read MVEX box")
PETRO_ERROR_TAG(invalid_mvhd_box, "Unable to read MVHD box")
PETRO_ERROR_TAG(invalid_nmhd_box, "Unable to read NMHD box")
PETRO_ERROR_TAG(invalid_padb_box, "Unable to read PADB box")
PETRO_ERROR_TAG(invalid_pdin_box, "Unable to read PDIN box")
PETRO_ERROR_TAG(invalid_pitm_box, "Unable to read PITM box")
PETRO_ERROR_TAG(invalid_root_box, "Unable to read ROOT box")
PETRO_ERROR_TAG(invalid_sbgp_box, "Unable to read SBGP box")
PETRO_ERROR_TAG(invalid_schi_box, "Unable to read SCHI box")
PETRO_ERROR_TAG(invalid_schm_box, "Unable to read SCHM box")
PETRO_ERROR_TAG(invalid_sdtp_box, "Unable to read SDTP box")
PETRO_ERROR_TAG(invalid_sgpd_box, "Unable to read SGPD box")
PETRO_ERROR_TAG(invalid_sinf_box, "Unable to read SINF box")
PETRO_ERROR_TAG(invalid_skip_box, "Unable to read SKIP box")
PETRO_ERROR_TAG(invalid_smhd_box, "Unable to read SMHD box")
PETRO_ERROR_TAG(invalid_stbl_box, "Unable to read STBL box")
PETRO_ERROR_TAG(invalid_stco_box, "Unable to read STCO box")
PETRO_ERROR_TAG(invalid_stdp_box, "Unable to read STDP box")
PETRO_ERROR_TAG(invalid_stsc_box, "Unable to read STSC box")
PETRO_ERROR_TAG(invalid_stsd_box, "Unable to read STSD box")
PETRO_ERROR_TAG(invalid_stsh_box, "Unable to read STSH box")
PETRO_ERROR_TAG(invalid_stss_box, "Unable to read STSS box")
PETRO_ERROR_TAG(invalid_stsz_box, "Unable to read STSZ box")
PETRO_ERROR_TAG(invalid_stts_box, "Unable to read STTS box")
PETRO_ERROR_TAG(invalid_stz2_box, "Unable to read STZ2 box")
PETRO_ERROR_TAG(invalid_subs_box, "Unable to read SUBS box")
PETRO_ERROR_TAG(invalid_tfhd_box, "Unable to read TFHD box")
PETRO_ERROR_TAG(invalid_tfra_box, "Unable to read TFRA box")
PETRO_ERROR_TAG(invalid_tkhd_box, "Unable to read TKHD box")
PETRO_ERROR_TAG(invalid_traf_box, "Unable to read TRAF box")
PETRO_ERROR_TAG(invalid_trak_box, "Unable to read TRAK box")
PETRO_ERROR_TAG(invalid_tref_box, "Unable to read TREF box")
PETRO_ERROR_TAG(invalid_trex_box, "Unable to read TREX box")
PETRO_ERROR_TAG(invalid_trun_box, "Unable to read TRUN box")
PETRO_ERROR_TAG(invalid_udta_box, "Unable to read UDTA box")
PETRO_ERROR_TAG(invalid_url_box, "Unable to read URL box")
PETRO_ERROR_TAG(invalid_urn_box, "Unable to read URN box")
PETRO_ERROR_TAG(invalid_vmhd_box, "Unable to read VMHD box")
PETRO_ERROR_TAG(invalid_xml_box, "Unable to read XML box")
PETRO_ERROR_TAG(invalid_sample_entry, "Unable to read sample entry")
PETRO_ERROR_TAG(invalid_sps, "Unable to read sequence parameter set")
PETRO_ERROR_TAG(mp4a_box_missing, "Unable to find mp4a box")
PETRO_ERROR_TAG(trak_box_missing, "Unable to find trak box")
PETRO_ERROR_TAG(esds_box_missing, "Unable to find esds box")
PETRO_ERROR_TAG(descriptor_missing, "Unable to find descriptor")
PETRO_ERROR_TAG(mvhd_box_missing, "Unable to find mvhd box")
PETRO_ERROR_TAG(mdhd_box_missing, "Unable to find mdhd box")
PETRO_ERROR_TAG(stts_box_missing, "Unable to find stts box")
PETRO_ERROR_TAG(co64_box_missing, "Unable to find co64 box")
PETRO_ERROR_TAG(stsc_box_missing, "Unable to find stsc box")
PETRO_ERROR_TAG(stsz_box_missing, "Unable to find stsz box")
PETRO_ERROR_TAG(avc1_box_missing, "Unable to find avc1 box")
PETRO_ERROR_TAG(avcc_box_missing, "Unable to find avcc box")
