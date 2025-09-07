set -e

WIICURL='https://github.com/AndrewPiroli/wii-curl/releases/download/c8.12.1%2Bm3.6.2/wii-curl-8.12.1-1-any.pkg.tar.gz'
WIISOCKET='https://github.com/AndrewPiroli/wii-curl/releases/download/c8.12.1%2Bm3.6.2/libwiisocket-0.1-1-any.pkg.tar.gz'
WIIMBEDTLS='https://github.com/AndrewPiroli/wii-curl/releases/download/c8.12.1%2Bm3.6.2/wii-mbedtls-3.6.2-2-any.pkg.tar.gz'

# make a temp directory for the libs and delete it on exit
INSTALL_DIR="$(mktemp -d)"
trap "rm -r $INSTALL_DIR" EXIT
cd $INSTALL_DIR

wget $WIICURL
wget $WIISOCKET
wget $WIIMBEDTLS

tar -xvf wii-curl-8.12.1-1-any.pkg.tar.gz
tar -xvf wii-mbedtls-3.6.2-2-any.pkg.tar.gz
tar -xvf libwiisocket-0.1-1-any.pkg.tar.gz

# the downloaded archive has a structure like `opt/devkitpro/portlibs/wii/{include,lib}/curl/...`, copy those files into `/opt/devkitpro/...`
rsync -av --remove-source-files opt/devkitpro/ /opt/devkitpro/
