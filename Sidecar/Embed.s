.section .rodata
    .global ftp_archive
	.type   ftp_archive, @object
	.align  4

ftp_archive:
    .incbin "OFTP10000.7z"
ftp_archive_End:
    .global ftp_archive_Size
    .type   ftp_archive_Size, @object
    .align  4
ftp_archive_Size:
    .int    ftp_archive_End - ftp_archive
