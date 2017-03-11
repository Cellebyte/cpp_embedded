FROM buildpack-deps:stretch

RUN set -x \
	&& apt-get update \
    && apt-get install -y gcc-6 g++-6 \
	&& rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/* \
    && apt-get autoremove -y \
    && apt-get clean

RUN useradd gc \
    && mkdir /home/gc \
    && chown gc /home/gc
