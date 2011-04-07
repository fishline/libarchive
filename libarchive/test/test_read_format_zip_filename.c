/*-
 * Copyright (c) 2011 Michihiro NAKAJIMA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "test.h"
__FBSDID("$FreeBSD");

#include <locale.h>

static void
test_read_format_zip_filename_CP932_eucJP(const char *refname)
{
	struct archive *a;
	struct archive_entry *ae;

	/*
	 * Read ZIP filename in ja_JP.eucJP with "hdrcharset=CP932" option.
	 */
	if (NULL == setlocale(LC_ALL, "ja_JP.eucJP")) {
		skipping("ja_JP.eucJP locale not available on this system.");
		return;
	}

	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_filter_all(a));
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_all(a));
	if (ARCHIVE_OK != archive_read_set_options(a, "hdrcharset=CP932")) {
		skipping("This system cannot convert character-set"
		    " from CP932 to eucJP.");
		goto cleanup;
	}
	assertEqualIntA(a, ARCHIVE_OK,
	    archive_read_open_filename(a, refname, 10240));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualString(
	    "\xc9\xbd\xa4\xc0\xa4\xe8\x2f\xb0\xec\xcd\xf7\xc9\xbd\x2e\x74\x78\x74",
	    archive_entry_pathname(ae));
	assertEqualInt(5, archive_entry_size(ae));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualString(
	    "\xc9\xbd\xa4\xc0\xa4\xe8\x2f\xb4\xc1\xbb\xfa\x2e\x74\x78\x74",
	    archive_entry_pathname(ae));
	assertEqualInt(5, archive_entry_size(ae));


	/* End of archive. */
	assertEqualIntA(a, ARCHIVE_EOF, archive_read_next_header(a, &ae));

	/* Verify archive format. */
	assertEqualIntA(a, ARCHIVE_COMPRESSION_NONE, archive_compression(a));
	assertEqualIntA(a, ARCHIVE_FORMAT_ZIP, archive_format(a));

	/* Close the archive. */
	assertEqualInt(ARCHIVE_OK, archive_read_close(a));
cleanup:
	assertEqualInt(ARCHIVE_OK, archive_read_free(a));
}

static void
test_read_format_zip_filename_CP932_UTF8(const char *refname)
{
	struct archive *a;
	struct archive_entry *ae;

	/*
	 * Read ZIP filename in ja_JP.UTF-8 with "hdrcharset=CP932" option.
	 */
	if (NULL == setlocale(LC_ALL, "ja_JP.UTF-8")) {
		skipping("ja_JP.UTF-8 locale not available on this system.");
		return;
	}

	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_filter_all(a));
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_all(a));
	if (ARCHIVE_OK != archive_read_set_options(a, "hdrcharset=CP932")) {
		skipping("This system cannot convert character-set"
		    " from CP932 to UTF-8.");
		goto cleanup;
	}
	assertEqualIntA(a, ARCHIVE_OK,
	    archive_read_open_filename(a, refname, 10240));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualString("\xe8\xa1\xa8\xe3\x81\xa0\xe3\x82\x88\x2f"
	    "\xe4\xb8\x80\xe8\xa6\xa7\xe8\xa1\xa8\x2e\x74\x78\x74",
	    archive_entry_pathname(ae));
	assertEqualInt(5, archive_entry_size(ae));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualString("\xe8\xa1\xa8\xe3\x81\xa0\xe3\x82\x88\x2f"
	    "\xe6\xbc\xa2\xe5\xad\x97\x2e\x74\x78\x74",
	    archive_entry_pathname(ae));
	assertEqualInt(5, archive_entry_size(ae));


	/* End of archive. */
	assertEqualIntA(a, ARCHIVE_EOF, archive_read_next_header(a, &ae));

	/* Verify archive format. */
	assertEqualIntA(a, ARCHIVE_COMPRESSION_NONE, archive_compression(a));
	assertEqualIntA(a, ARCHIVE_FORMAT_ZIP, archive_format(a));

	/* Close the archive. */
	assertEqualInt(ARCHIVE_OK, archive_read_close(a));
cleanup:
	assertEqualInt(ARCHIVE_OK, archive_read_free(a));
}

static void
test_read_format_zip_filename_UTF8_eucJP(const char *refname)
{
	struct archive *a;
	struct archive_entry *ae;

	/*
	 * Read ZIP filename in ja_JP.eucJP without charset option
	 * because the file name in the sample file is UTF-8 and
	 * Bit 11 of its general purpose bit flag is set.
	 */
	if (NULL == setlocale(LC_ALL, "ja_JP.eucJP")) {
		skipping("ja_JP.eucJP locale not available on this system.");
		return;
	}
	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_zip(a));
	if (ARCHIVE_OK != archive_read_set_options(a, "hdrcharset=UTF-8")) {
		skipping("This system cannot convert character-set"
		    " from UTF-8 to eucJP.");
		goto cleanup;
	}
	assertEqualInt(ARCHIVE_OK, archive_read_free(a));

	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_filter_all(a));
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_all(a));
	assertEqualIntA(a, ARCHIVE_OK,
	    archive_read_open_filename(a, refname, 10240));

	/* Verify directory file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualInt(AE_IFDIR, archive_entry_filetype(ae));
	assertEqualString("\xc9\xbd\xa4\xc0\xa4\xe8\x2f",
	    archive_entry_pathname(ae));
	assertEqualInt(0, archive_entry_size(ae));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualInt(AE_IFREG, archive_entry_filetype(ae));
	assertEqualString(
	    "\xc9\xbd\xa4\xc0\xa4\xe8\x2f\xb0\xec\xcd\xf7\xc9\xbd\x2e\x74\x78\x74",
	    archive_entry_pathname(ae));
	assertEqualInt(5, archive_entry_size(ae));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualInt(AE_IFREG, archive_entry_filetype(ae));
	assertEqualString(
	    "\xc9\xbd\xa4\xc0\xa4\xe8\x2f\xb4\xc1\xbb\xfa\x2e\x74\x78\x74",
	    archive_entry_pathname(ae));
	assertEqualInt(5, archive_entry_size(ae));


	/* End of archive. */
	assertEqualIntA(a, ARCHIVE_EOF, archive_read_next_header(a, &ae));

	/* Verify archive format. */
	assertEqualIntA(a, ARCHIVE_COMPRESSION_NONE, archive_compression(a));
	assertEqualIntA(a, ARCHIVE_FORMAT_ZIP, archive_format(a));

	/* Close the archive. */
	assertEqualInt(ARCHIVE_OK, archive_read_close(a));
cleanup:
	assertEqualInt(ARCHIVE_OK, archive_read_free(a));
}

static void
test_read_format_zip_filename_UTF8_UTF8(const char *refname)
{
	struct archive *a;
	struct archive_entry *ae;

	/*
	 * Read ZIP filename in ja_JP.UTF-8 without charset option
	 * because the file name in the sample file is UTF-8 and
	 * Bit 11 of its general purpose bit flag is set.
	 */
	if (NULL == setlocale(LC_ALL, "ja_JP.UTF-8")) {
		skipping("ja_JP.UTF-8 locale not available on this system.");
		return;
	}

	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_filter_all(a));
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_all(a));
	assertEqualIntA(a, ARCHIVE_OK,
	    archive_read_open_filename(a, refname, 10240));

	/* Verify directory file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualInt(AE_IFDIR, archive_entry_filetype(ae));
	assertEqualString("\xe8\xa1\xa8\xe3\x81\xa0\xe3\x82\x88\x2f",
	    archive_entry_pathname(ae));
	assertEqualInt(0, archive_entry_size(ae));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualInt(AE_IFREG, archive_entry_filetype(ae));
	assertEqualString("\xe8\xa1\xa8\xe3\x81\xa0\xe3\x82\x88\x2f"
	    "\xe4\xb8\x80\xe8\xa6\xa7\xe8\xa1\xa8\x2e\x74\x78\x74",
	    archive_entry_pathname(ae));
	assertEqualInt(5, archive_entry_size(ae));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualInt(AE_IFREG, archive_entry_filetype(ae));
	assertEqualString("\xe8\xa1\xa8\xe3\x81\xa0\xe3\x82\x88\x2f"
	    "\xe6\xbc\xa2\xe5\xad\x97\x2e\x74\x78\x74",
	    archive_entry_pathname(ae));
	assertEqualInt(5, archive_entry_size(ae));


	/* End of archive. */
	assertEqualIntA(a, ARCHIVE_EOF, archive_read_next_header(a, &ae));

	/* Verify archive format. */
	assertEqualIntA(a, ARCHIVE_COMPRESSION_NONE, archive_compression(a));
	assertEqualIntA(a, ARCHIVE_FORMAT_ZIP, archive_format(a));

	/* Close the archive. */
	assertEqualInt(ARCHIVE_OK, archive_read_close(a));
	assertEqualInt(ARCHIVE_OK, archive_read_free(a));
}

static void
test_read_format_zip_filename_CP866_KOI8R(const char *refname)
{
	struct archive *a;
	struct archive_entry *ae;

	/*
	 * Read ZIP filename in ru_RU.KOI8-R with "hdrcharset=CP866" option.
	 */
	if (NULL == setlocale(LC_ALL, "ru_RU.KOI8-R")) {
		skipping("ru_RU.KOI8-R locale not available on this system.");
		return;
	}

	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_filter_all(a));
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_all(a));
	if (ARCHIVE_OK != archive_read_set_options(a, "hdrcharset=CP866")) {
		skipping("This system cannot convert character-set"
		    " from CP866 to KOI8-R.");
		goto cleanup;
	}
	assertEqualIntA(a, ARCHIVE_OK,
	    archive_read_open_filename(a, refname, 10240));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualString("\xf0\xf2\xe9\xf7\xe5\xf4",
	    archive_entry_pathname(ae));
	assertEqualInt(6, archive_entry_size(ae));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualString("\xd0\xd2\xc9\xd7\xc5\xd4",
	    archive_entry_pathname(ae));
	assertEqualInt(6, archive_entry_size(ae));


	/* End of archive. */
	assertEqualIntA(a, ARCHIVE_EOF, archive_read_next_header(a, &ae));

	/* Verify archive format. */
	assertEqualIntA(a, ARCHIVE_COMPRESSION_NONE, archive_compression(a));
	assertEqualIntA(a, ARCHIVE_FORMAT_ZIP, archive_format(a));

	/* Close the archive. */
	assertEqualInt(ARCHIVE_OK, archive_read_close(a));
cleanup:
	assertEqualInt(ARCHIVE_OK, archive_read_free(a));
}

static void
test_read_format_zip_filename_CP866_UTF8(const char *refname)
{
	struct archive *a;
	struct archive_entry *ae;

	/*
	 * Read ZIP filename in ru_RU.UTF-8 with "hdrcharset=CP866" option.
	 */
	if (NULL == setlocale(LC_ALL, "ru_RU.UTF-8")) {
		skipping("ru_RU.UTF-8 locale not available on this system.");
		return;
	}

	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_filter_all(a));
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_all(a));
	if (ARCHIVE_OK != archive_read_set_options(a, "hdrcharset=CP866")) {
		skipping("This system cannot convert character-set"
		    " from CP866 to UTF-8.");
		goto cleanup;
	}
	assertEqualIntA(a, ARCHIVE_OK,
	    archive_read_open_filename(a, refname, 10240));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualString("\xd0\x9f\xd0\xa0\xd0\x98\xd0\x92\xd0\x95\xd0\xa2",
	    archive_entry_pathname(ae));
	assertEqualInt(6, archive_entry_size(ae));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualString("\xd0\xbf\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82",
	    archive_entry_pathname(ae));
	assertEqualInt(6, archive_entry_size(ae));


	/* End of archive. */
	assertEqualIntA(a, ARCHIVE_EOF, archive_read_next_header(a, &ae));

	/* Verify archive format. */
	assertEqualIntA(a, ARCHIVE_COMPRESSION_NONE, archive_compression(a));
	assertEqualIntA(a, ARCHIVE_FORMAT_ZIP, archive_format(a));

	/* Close the archive. */
	assertEqualInt(ARCHIVE_OK, archive_read_close(a));
cleanup:
	assertEqualInt(ARCHIVE_OK, archive_read_free(a));
}

static void
test_read_format_zip_filename_KOI8R_CP866(const char *refname)
{
	struct archive *a;
	struct archive_entry *ae;

	/*
	 * Read ZIP filename in ru_RU.CP866 with "hdrcharset=KOI8-R" option.
	 */
	if (NULL == setlocale(LC_ALL, "ru_RU.CP866")) {
		skipping("ru_RU.CP866 locale not available on this system.");
		return;
	}

	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_filter_all(a));
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_all(a));
	if (ARCHIVE_OK != archive_read_set_options(a, "hdrcharset=KOI8-R")) {
		skipping("This system cannot convert character-set"
		    " from KOI8-R to CP866.");
		goto cleanup;
	}
	assertEqualIntA(a, ARCHIVE_OK,
	    archive_read_open_filename(a, refname, 10240));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualString("\xaf\xe0\xa8\xa2\xa5\xe2",
	    archive_entry_pathname(ae));
	assertEqualInt(6, archive_entry_size(ae));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualString("\x8f\x90\x88\x82\x85\x92",
	    archive_entry_pathname(ae));
	assertEqualInt(6, archive_entry_size(ae));


	/* End of archive. */
	assertEqualIntA(a, ARCHIVE_EOF, archive_read_next_header(a, &ae));

	/* Verify archive format. */
	assertEqualIntA(a, ARCHIVE_COMPRESSION_NONE, archive_compression(a));
	assertEqualIntA(a, ARCHIVE_FORMAT_ZIP, archive_format(a));

	/* Close the archive. */
	assertEqualInt(ARCHIVE_OK, archive_read_close(a));
cleanup:
	assertEqualInt(ARCHIVE_OK, archive_read_free(a));
}

static void
test_read_format_zip_filename_KOI8R_UTF8(const char *refname)
{
	struct archive *a;
	struct archive_entry *ae;

	/*
	 * Read ZIP filename in ru_RU.UTF-8 with "hdrcharset=KOI8-R" option.
	 */
	if (NULL == setlocale(LC_ALL, "ru_RU.UTF-8")) {
		skipping("ru_RU.UTF-8 locale not available on this system.");
		return;
	}

	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_filter_all(a));
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_all(a));
	if (ARCHIVE_OK != archive_read_set_options(a, "hdrcharset=KOI8-R")) {
		skipping("This system cannot convert character-set"
		    " from KOI8-R to UTF-8.");
		goto cleanup;
	}
	assertEqualIntA(a, ARCHIVE_OK,
	    archive_read_open_filename(a, refname, 10240));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualString("\xd0\xbf\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82",
	    archive_entry_pathname(ae));
	assertEqualInt(6, archive_entry_size(ae));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualString("\xd0\x9f\xd0\xa0\xd0\x98\xd0\x92\xd0\x95\xd0\xa2",
	    archive_entry_pathname(ae));
	assertEqualInt(6, archive_entry_size(ae));


	/* End of archive. */
	assertEqualIntA(a, ARCHIVE_EOF, archive_read_next_header(a, &ae));

	/* Verify archive format. */
	assertEqualIntA(a, ARCHIVE_COMPRESSION_NONE, archive_compression(a));
	assertEqualIntA(a, ARCHIVE_FORMAT_ZIP, archive_format(a));

	/* Close the archive. */
	assertEqualInt(ARCHIVE_OK, archive_read_close(a));
cleanup:
	assertEqualInt(ARCHIVE_OK, archive_read_free(a));
}

static void
test_read_format_zip_filename_UTF8_KOI8R(const char *refname)
{
	struct archive *a;
	struct archive_entry *ae;

	/*
	 * Read ZIP filename in ru_RU.KOI8-R with "hdrcharset=UTF-8" option.
	 */
	if (NULL == setlocale(LC_ALL, "ru_RU.KOI8-R")) {
		skipping("ru_RU.KOI8-R locale not available on this system.");
		return;
	}

	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_all(a));
	if (ARCHIVE_OK != archive_read_set_options(a, "hdrcharset=UTF-8")) {
		skipping("This system cannot convert character-set"
		    " from UTF-8 to KOI8-R.");
		goto cleanup;
	}
	assertEqualInt(ARCHIVE_OK, archive_read_free(a));

	/* Re-create a read archive object. */
	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_filter_all(a));
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_all(a));
	assertEqualIntA(a, ARCHIVE_OK,
	    archive_read_open_filename(a, refname, 10240));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualString("\xf0\xf2\xe9\xf7\xe5\xf4",
	    archive_entry_pathname(ae));
	assertEqualInt(6, archive_entry_size(ae));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualString("\xd0\xd2\xc9\xd7\xc5\xd4",
	    archive_entry_pathname(ae));
	assertEqualInt(6, archive_entry_size(ae));


	/* End of archive. */
	assertEqualIntA(a, ARCHIVE_EOF, archive_read_next_header(a, &ae));

	/* Verify archive format. */
	assertEqualIntA(a, ARCHIVE_COMPRESSION_NONE, archive_compression(a));
	assertEqualIntA(a, ARCHIVE_FORMAT_ZIP, archive_format(a));

	/* Close the archive. */
	assertEqualInt(ARCHIVE_OK, archive_read_close(a));
cleanup:
	assertEqualInt(ARCHIVE_OK, archive_read_free(a));
}

static void
test_read_format_zip_filename_UTF8_CP866(const char *refname)
{
	struct archive *a;
	struct archive_entry *ae;

	/*
	 * Read ZIP filename in ru_RU.CP866 without charset option
	 * because the file name in the sample file is UTF-8 and
	 * Bit 11 of its general purpose bit flag is set.
	 */
	if (NULL == setlocale(LC_ALL, "ru_RU.CP866")) {
		skipping("ru_RU.CP866 locale not available on this system.");
		return;
	}

	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_filter_all(a));
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_all(a));
	if (ARCHIVE_OK != archive_read_set_options(a, "hdrcharset=UTF-8")) {
		skipping("This system cannot convert character-set"
		    " from UTF-8 to CP866.");
		goto cleanup;
	}
	assertEqualInt(ARCHIVE_OK, archive_read_free(a));

	/* Re-create a read archive object. */
	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_filter_all(a));
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_all(a));
	assertEqualIntA(a, ARCHIVE_OK,
	    archive_read_open_filename(a, refname, 10240));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualString("\x8f\x90\x88\x82\x85\x92",
	    archive_entry_pathname(ae));
	assertEqualInt(6, archive_entry_size(ae));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualString("\xaf\xe0\xa8\xa2\xa5\xe2",
	    archive_entry_pathname(ae));
	assertEqualInt(6, archive_entry_size(ae));


	/* End of archive. */
	assertEqualIntA(a, ARCHIVE_EOF, archive_read_next_header(a, &ae));

	/* Verify archive format. */
	assertEqualIntA(a, ARCHIVE_COMPRESSION_NONE, archive_compression(a));
	assertEqualIntA(a, ARCHIVE_FORMAT_ZIP, archive_format(a));

	/* Close the archive. */
	assertEqualInt(ARCHIVE_OK, archive_read_close(a));
cleanup:
	assertEqualInt(ARCHIVE_OK, archive_read_free(a));
}

static void
test_read_format_zip_filename_UTF8_UTF8_ru(const char *refname)
{
	struct archive *a;
	struct archive_entry *ae;

	/*
	 * Read ZIP filename in ru_RU.UTF-8 without charset option
	 * because the file name in the sample file is UTF-8 and
	 * Bit 11 of its general purpose bit flag is set.
	 */
	if (NULL == setlocale(LC_ALL, "ru_RU.UTF-8")) {
		skipping("ru_RU.UTF-8 locale not available on this system.");
		return;
	}

	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_filter_all(a));
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_all(a));
	assertEqualIntA(a, ARCHIVE_OK,
	    archive_read_open_filename(a, refname, 10240));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualString("\xd0\x9f\xd0\xa0\xd0\x98\xd0\x92\xd0\x95\xd0\xa2",
	    archive_entry_pathname(ae));
	assertEqualInt(6, archive_entry_size(ae));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualString("\xd0\xbf\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82",
	    archive_entry_pathname(ae));
	assertEqualInt(6, archive_entry_size(ae));


	/* End of archive. */
	assertEqualIntA(a, ARCHIVE_EOF, archive_read_next_header(a, &ae));

	/* Verify archive format. */
	assertEqualIntA(a, ARCHIVE_COMPRESSION_NONE, archive_compression(a));
	assertEqualIntA(a, ARCHIVE_FORMAT_ZIP, archive_format(a));

	/* Close the archive. */
	assertEqualInt(ARCHIVE_OK, archive_read_close(a));
	assertEqualInt(ARCHIVE_OK, archive_read_free(a));
}

#if defined(_WIN32) && !defined(__CYGWIN__)

/*
 * Filenames conversion test on Windows.
 * We have to compare filenames in WCS because setlocale() can affects 
 * only CRT functins, and Windows uses escape character to compose
 * several character-set in any locale so that many locale-dependent
 * MBS can be together displayed on Console. So MBS can be described
 * by different byte sequence if the current locale is different.
 */

static void
test_read_format_zip_filename_CP932_WIN(const char *refname)
{
	struct archive *a;
	struct archive_entry *ae;

	/*
	 * Read ZIP filename in ja_JP.eucJP with "hdrcharset=CP932" option.
	 */
	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_filter_all(a));
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_all(a));
	if (ARCHIVE_OK != archive_read_set_options(a, "hdrcharset=CP932")) {
		skipping("This system cannot convert character-set"
		    " from CP932 to eucJP.");
		goto cleanup;
	}
	assertEqualIntA(a, ARCHIVE_OK,
	    archive_read_open_filename(a, refname, 10240));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualWString(
	    L"\u8868\u3060\u3088/\u4e00\u89a7\u8868.txt",
	    archive_entry_pathname_w(ae));
	assertEqualInt(5, archive_entry_size(ae));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualWString(
	    L"\u8868\u3060\u3088/\u6f22\u5b57.txt",
	    archive_entry_pathname_w(ae));
	assertEqualInt(5, archive_entry_size(ae));


	/* End of archive. */
	assertEqualIntA(a, ARCHIVE_EOF, archive_read_next_header(a, &ae));

	/* Verify archive format. */
	assertEqualIntA(a, ARCHIVE_COMPRESSION_NONE, archive_compression(a));
	assertEqualIntA(a, ARCHIVE_FORMAT_ZIP, archive_format(a));

	/* Close the archive. */
	assertEqualInt(ARCHIVE_OK, archive_read_close(a));
cleanup:
	assertEqualInt(ARCHIVE_OK, archive_read_free(a));
}

static void
test_read_format_zip_filename_UTF8_jp_WIN(const char *refname)
{
	struct archive *a;
	struct archive_entry *ae;

	/*
	 * Read ZIP filename in ja_JP.eucJP without charset option
	 * because the file name in the sample file is UTF-8 and
	 * Bit 11 of its general purpose bit flag is set.
	 */
	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_filter_all(a));
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_all(a));
	assertEqualIntA(a, ARCHIVE_OK,
	    archive_read_open_filename(a, refname, 10240));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualInt(AE_IFDIR, archive_entry_filetype(ae));
	assertEqualWString(
	    L"\u8868\u3060\u3088/",
	    archive_entry_pathname_w(ae));
	assertEqualInt(0, archive_entry_size(ae));

	/* Verify directory file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualInt(AE_IFREG, archive_entry_filetype(ae));
	assertEqualWString(
	    L"\u8868\u3060\u3088/\u4e00\u89a7\u8868.txt",
	    archive_entry_pathname_w(ae));
	assertEqualInt(5, archive_entry_size(ae));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualInt(AE_IFREG, archive_entry_filetype(ae));
	assertEqualWString(
	    L"\u8868\u3060\u3088/\u6f22\u5b57.txt",
	    archive_entry_pathname_w(ae));
	assertEqualInt(5, archive_entry_size(ae));

	/* End of archive. */
	assertEqualIntA(a, ARCHIVE_EOF, archive_read_next_header(a, &ae));

	/* Verify archive format. */
	assertEqualIntA(a, ARCHIVE_COMPRESSION_NONE, archive_compression(a));
	assertEqualIntA(a, ARCHIVE_FORMAT_ZIP, archive_format(a));

	/* Close the archive. */
	assertEqualInt(ARCHIVE_OK, archive_read_close(a));
	assertEqualInt(ARCHIVE_OK, archive_read_free(a));
}

static void
test_read_format_zip_filename_CP866_WIN(const char *refname)
{
	struct archive *a;
	struct archive_entry *ae;

	/*
	 * Read ZIP filename in ru_RU.KOI8-R with "hdrcharset=CP866" option.
	 */
	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_filter_all(a));
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_all(a));
	if (ARCHIVE_OK != archive_read_set_options(a, "hdrcharset=CP866")) {
		skipping("This system cannot convert character-set"
		    " from CP866.");
		goto cleanup;
	}
	assertEqualIntA(a, ARCHIVE_OK,
	    archive_read_open_filename(a, refname, 10240));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualWString(L"\u041f\u0420\u0418\u0412\u0415\u0422",
	    archive_entry_pathname_w(ae));
	assertEqualInt(6, archive_entry_size(ae));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualWString(L"\u043f\u0440\u0438\u0432\u0435\u0442",
	    archive_entry_pathname_w(ae));
	assertEqualInt(6, archive_entry_size(ae));


	/* End of archive. */
	assertEqualIntA(a, ARCHIVE_EOF, archive_read_next_header(a, &ae));

	/* Verify archive format. */
	assertEqualIntA(a, ARCHIVE_COMPRESSION_NONE, archive_compression(a));
	assertEqualIntA(a, ARCHIVE_FORMAT_ZIP, archive_format(a));

	/* Close the archive. */
	assertEqualInt(ARCHIVE_OK, archive_read_close(a));
cleanup:
	assertEqualInt(ARCHIVE_OK, archive_read_free(a));
}

static void
test_read_format_zip_filename_KOI8R_WIN(const char *refname)
{
	struct archive *a;
	struct archive_entry *ae;

	/*
	 * Read ZIP filename in ru_RU.CP866 with "hdrcharset=KOI8-R" option.
	 */
	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_filter_all(a));
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_all(a));
	if (ARCHIVE_OK != archive_read_set_options(a, "hdrcharset=KOI8-R")) {
		skipping("This system cannot convert character-set"
		    " from KOI8-R.");
		goto cleanup;
	}
	assertEqualIntA(a, ARCHIVE_OK,
	    archive_read_open_filename(a, refname, 10240));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualWString(L"\u043f\u0440\u0438\u0432\u0435\u0442",
	    archive_entry_pathname_w(ae));
	assertEqualInt(6, archive_entry_size(ae));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualWString(L"\u041f\u0420\u0418\u0412\u0415\u0422",
	    archive_entry_pathname_w(ae));
	assertEqualInt(6, archive_entry_size(ae));


	/* End of archive. */
	assertEqualIntA(a, ARCHIVE_EOF, archive_read_next_header(a, &ae));

	/* Verify archive format. */
	assertEqualIntA(a, ARCHIVE_COMPRESSION_NONE, archive_compression(a));
	assertEqualIntA(a, ARCHIVE_FORMAT_ZIP, archive_format(a));

	/* Close the archive. */
	assertEqualInt(ARCHIVE_OK, archive_read_close(a));
cleanup:
	assertEqualInt(ARCHIVE_OK, archive_read_free(a));
}

static void
test_read_format_zip_filename_UTF8_ru_WIN(const char *refname)
{
	struct archive *a;
	struct archive_entry *ae;

	/*
	 * Read ZIP filename in ru_RU.UTF-8 without charset option
	 * because the file name in the sample file is UTF-8 and
	 * Bit 11 of its general purpose bit flag is set.
	 */
	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_filter_all(a));
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_all(a));
	assertEqualIntA(a, ARCHIVE_OK,
	    archive_read_open_filename(a, refname, 10240));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualWString(L"\u041f\u0420\u0418\u0412\u0415\u0422",
	    archive_entry_pathname_w(ae));
	assertEqualInt(6, archive_entry_size(ae));

	/* Verify regular file. */
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	assertEqualWString(L"\u043f\u0440\u0438\u0432\u0435\u0442",
	    archive_entry_pathname_w(ae));
	assertEqualInt(6, archive_entry_size(ae));


	/* End of archive. */
	assertEqualIntA(a, ARCHIVE_EOF, archive_read_next_header(a, &ae));

	/* Verify archive format. */
	assertEqualIntA(a, ARCHIVE_COMPRESSION_NONE, archive_compression(a));
	assertEqualIntA(a, ARCHIVE_FORMAT_ZIP, archive_format(a));

	/* Close the archive. */
	assertEqualInt(ARCHIVE_OK, archive_read_close(a));
	assertEqualInt(ARCHIVE_OK, archive_read_free(a));
}

#endif

DEFINE_TEST(test_read_format_zip_filename)
{
	const char *refname = "test_read_format_zip_cp932.zip";
	const char *refname2 = "test_read_format_zip_utf8.zip";
	const char *refname3 = "test_read_format_zip_cp866.zip";
	const char *refname4 = "test_read_format_zip_koi8r.zip";
	const char *refname5 = "test_read_format_zip_utf8_ru.zip";

	extract_reference_file(refname);
	test_read_format_zip_filename_CP932_eucJP(refname);
	test_read_format_zip_filename_CP932_UTF8(refname);
	extract_reference_file(refname2);
	test_read_format_zip_filename_UTF8_eucJP(refname2);
	test_read_format_zip_filename_UTF8_UTF8(refname2);
	extract_reference_file(refname3);
	test_read_format_zip_filename_CP866_KOI8R(refname3);
	test_read_format_zip_filename_CP866_UTF8(refname3);
	extract_reference_file(refname4);
	test_read_format_zip_filename_KOI8R_CP866(refname4);
	test_read_format_zip_filename_KOI8R_UTF8(refname4);
	extract_reference_file(refname5);
	test_read_format_zip_filename_UTF8_KOI8R(refname5);
	test_read_format_zip_filename_UTF8_CP866(refname5);
	test_read_format_zip_filename_UTF8_UTF8_ru(refname5);
#if defined(_WIN32) && !defined(__CYGWIN__)
	test_read_format_zip_filename_CP932_WIN(refname);
	test_read_format_zip_filename_UTF8_jp_WIN(refname2);
	test_read_format_zip_filename_CP866_WIN(refname3);
	test_read_format_zip_filename_KOI8R_WIN(refname4);
	test_read_format_zip_filename_UTF8_ru_WIN(refname5);
#endif
}
