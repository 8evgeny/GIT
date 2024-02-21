FROM ubuntu:latest
ENV DEBIAN_FRONTEND noninteractive.  #не задавать вопросов
RUN apt-get update && apt-get upgrade -y && apt-get install -y language-pack-ru qtbase5-dev cmake p7zip-full build-essential qrencode tzdata python3-pip bash libpqxx-dev
ENV LANGUAGE ru_RU.UTF-8
ENV LANG ru_RU.UTF-8
ENV LC_ALL ru_RU.UTF-8
RUN locale-gen ru_RU.UTF-8 && dpkg-reconfigure locales
RUN groupadd -r evg && useradd -r -g evg evg
USER evg
WORKDIR /home/evg/SOFT/Github/GIT
RUN mkdir -p /home/evg/SOFT/Github/GIT/16_WEB_ARHIV
#RUN pip install conan 
#RUN conan install ./16_WEB_ARHIV/create_content_for_web --profile profile
COPY --chown=evg:evg ./CONTENT /home/evg/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT
COPY --chown=evg:evg ./create_content_for_web /home/evg/SOFT/Github/GIT/16_WEB_ARHIV/create_content_for_web
COPY --chown=evg:evg ./NIOKR_aktual_DOC /home/evg/SOFT/Github/GIT/16_WEB_ARHIV/NIOKR_aktual_DOC
COPY --chown=evg:evg ./NIOKR_IUL /home/evg/SOFT/Github/GIT/16_WEB_ARHIV/NIOKR_IUL
COPY --chown=evg:evg ./NIOKR_messages /home/evg/SOFT/Github/GIT/16_WEB_ARHIV/NIOKR_messages
COPY --chown=evg:evg ./NIOKR_old_DOC /home/evg/SOFT/Github/GIT/16_WEB_ARHIV/NIOKR_old_DOC
RUN cd ./16_WEB_ARHIV/create_content_for_web/build && rm CMakeCache.txt && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j8 all
CMD ["16_WEB_ARHIV/create_content_for_web/build/bin/creater_web_content"]
