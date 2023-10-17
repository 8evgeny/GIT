FROM ubuntu:latest
#не задавать вопросов
ENV DEBIAN_FRONTEND noninteractive.
RUN apt-get update && apt-get upgrade -y && apt-get install -y language-pack-ru qtbase5-dev cmake p7zip-full build-essential qrencode
ENV LANGUAGE ru_RU.UTF-8
ENV LANG ru_RU.UTF-8
ENV LC_ALL ru_RU.UTF-8
RUN locale-gen ru_RU.UTF-8 && dpkg-reconfigure locales && rm -rf /etc/localtime && ln -s /usr/share/zoneinfo/Europe/Moskow /etc/localtime
COPY ./create_content_for_web /app/create_content_for_web
#COPY ./CONTENT /app/CONTENT
#COPY ./Ниокр-Актуальные_документы /app/Ниокр-Актуальные_документы
#COPY ./Ниокр-Документы_по_обозначениям /app/Ниокр-Документы_по_обозначениям
#COPY ./Ниокр-Извещения_об_изменении /app/Ниокр-Извещения_об_изменении
#COPY ./Ниокр-Неактуальные_документы /app/Ниокр-Неактуальные_документы
WORKDIR /app/create_content_for_web/build
RUN rm CMakeCache.txt && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j8 all
CMD ["./createWebContent"]
