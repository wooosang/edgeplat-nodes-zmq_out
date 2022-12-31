FROM ubuntu:18.04

RUN sed -i s:/archive.ubuntu.com:/mirrors.tuna.tsinghua.edu.cn/ubuntu:g /etc/apt/sources.list
RUN cat /etc/apt/sources.list
RUN apt-get clean
RUN apt-get -y update --fix-missing
RUN apt install -y wget zip cmake git
RUN wget https://github.com/opencv/opencv/archive/3.2.0.zip -O opencv-3.2.0.zip
RUN apt install -y  build-essential pkg-config libgtk2.0-dev 
RUN apt update -y && apt upgrade -y && apt install -y libavformat-dev libswscale-dev libavcodec-dev 
RUN apt update -y && apt install -y python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-22-dev
RUN apt install -y software-properties-common
RUN add-apt-repository "deb http://security.ubuntu.com/ubuntu xenial-security main"
RUN apt update -y && apt install -y libjasper1 libjasper-dev
RUN cd /tmp && wget https://github.com/opencv/opencv/archive/3.2.0.zip -O opencv-3.2.0.zip && unzip opencv-3.2.0.zip
#RUN ls /tmp/opencv-3.2.0/3rdparty/ippicv/downloads
COPY ippicv_linux_20151201.tgz  /tmp/opencv-3.2.0/3rdparty/ippicv/downloads/linux-808b791a6eac9ed78d32a7666804320e/
RUN cd /tmp/opencv-3.2.0 && mkdir build && cd build && cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..
RUN cd /tmp/opencv-3.2.0/build && make -j4 && make install 
RUN echo '/usr/local/lib' > /etc/ld.so.conf.d/opencv.conf && ldconfig 
RUN echo 'PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig \nexport PKG_CONFIG_PATH' >> /etc/bash.bashrc  
RUN apt install -y mlocate
RUN /bin/bash -c  "source /etc/bash.bashrc" &&  /bin/bash -c updatedb 
ENV LD_LIBRARY_PATH=/opt/edgeplat/lib:/opt/halcon/lib/x64-linux

RUN mkdir -p /opt/halcon && mkdir -p /opt/halcon/license /opt/halcon/lib /opt/halcon/include
COPY license/* /opt/halcon/license/
COPY include.tar.gz /opt/halcon/include/
COPY lib/x64-linux /opt/halcon/lib/x64-linux/
RUN echo 'HALCONARCH=x64-linux; export HALCONARCH \nHALCONROOT=/opt/halcon; export HALCONROOT \nHALCONEXAMPLES=${HALCONROOT}/examples; export HALCONEXAMPLES \nHALCONIMAGES=${HALCONROOT}/examples/images; export HALCONIMAGES \n PATH=${HALCONROOT}/bin/${HALCONARCH}:${HALCONROOT}/FLEXlm/${HALCONARCH}:${PATH}\nexport PATH\nif [ ${LD_LIBRARY_PATH} ] ; then\nLD_LIBRARY_PATH=${HALCONROOT}/lib/${HALCONARCH}:${LD_LIBRARY_PATH}\nexport LD_LIBRARY_PATH\nelse\nLD_LIBRARY_PATH=${HALCONROOT}/lib/${HALCONARCH}; export LD_LIBRARY_PATH\nfi\nif [ "x${FLEXID_LIBRARY_PATH}" = "x" ]; then\nFLEXID_LIBRARY_PATH="${HALCONROOT}/FLEXlm/${HALCONARCH}/flexid9:/usr/lib"\nexport FLEXID_LIBRARY_PATH\nfi\nexport LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/lib:/usr/lib/x86_64-linux-gun/cudart:/lib:/usr/lib/x86_64-linux-gun/nvrtc\nexport LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/lib:/usr/lib/x86_64-linux-gnu/blas:/lib:/usr/lib/x86_64-linux-gnu/lapack' >> ~/.bashrc
RUN /bin/bash -c "source ~/.bashrc"
RUN apt install -y vim gdb

COPY edgeplat_lib/* /opt/edgeplat/lib/
ADD defect_checker /app/checker
WORKDIR /app
COPY edgeplat_lib/libevl.so /app/
ENTRYPOINT ["/app/checker", "-p", "8080"]
