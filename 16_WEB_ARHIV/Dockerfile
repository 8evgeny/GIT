FROM ubuntu:latest
ENV DEBIAN_FRONTEND noninteractive.  #не задавать вопросов
RUN apt-get update && apt-get upgrade -y && apt-get install -y language-pack-ru qtbase5-dev cmake p7zip-full build-essential qrencode tzdata
ENV LANGUAGE ru_RU.UTF-8
ENV LANG ru_RU.UTF-8
ENV LC_ALL ru_RU.UTF-8
RUN locale-gen ru_RU.UTF-8 && dpkg-reconfigure locales
RUN groupadd -r evg && useradd -r -g evg evg
USER evg
WORKDIR /home/evg/SOFT/Github/GIT
RUN mkdir -p /home/evg/SOFT/Github/GIT/16_WEB_ARHIV
COPY --chown=evg:evg ./CONTENT /home/evg/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT
COPY --chown=evg:evg ./create_content_for_web /home/evg/SOFT/Github/GIT/16_WEB_ARHIV/create_content_for_web
COPY --chown=evg:evg ./Ниокр-Актуальные_документы /home/evg/SOFT/Github/GIT/16_WEB_ARHIV/Ниокр-Актуальные_документы
COPY --chown=evg:evg ./Ниокр-Документы_по_обозначениям /home/evg/SOFT/Github/GIT/16_WEB_ARHIV/Ниокр-Документы_по_обозначениям
COPY --chown=evg:evg ./Ниокр-Извещения_об_изменении /home/evg/SOFT/Github/GIT/16_WEB_ARHIV/Ниокр-Извещения_об_изменении
COPY --chown=evg:evg ./Ниокр-Неактуальные_документы /home/evg/SOFT/Github/GIT/16_WEB_ARHIV/Ниокр-Неактуальные_документы
RUN cd ./16_WEB_ARHIV/create_content_for_web/build && rm CMakeCache.txt && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j8 all
CMD ["16_WEB_ARHIV/create_content_for_web/build/createWebContent"]
