echo "Creating disc image..."
mkdir CDROM
c:\psyq\bin\OtherTools\buildcd\buildcd -l flppybir.cti -icdrom\FLPPYBIR.IMG
del *.TOC
c:\psyq\cdemu\bin\stripiso s 2352 cdrom\flppybir.img cdrom\flppybir.iso
c:\psyq\DiscPatcher3\discpatcher