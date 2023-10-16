FROM gcc:latest
COPY ./CONTENT /app/CONTENT
COPY ./create_content_for_web /app/create_content_for_web
WORKDIR /app
RUN apt-get update && apt-get install -y qtbase5-dev cmake && mkdir /app/Ниокр-Актуальные\ документы && mkdir /app/Ниокр-Документы\ по\ обозначениям && mkdir /app/Ниокр-Извещения\ об\ изменении && cd /app/create_content_for_web/build && rm CMakeCache.txt && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j8 all 
WORKDIR /app/create_content_for_web/build
CMD ["./createWebContent"]
