FROM gcc:latest
COPY ./CONTENT /app/CONTENT
COPY ./create_content_for_web /app/create_content_for_web
COPY ./'Ниокр-Актуальные документы' /app/'Ниокр-Актуальные документы'
COPY ./'Ниокр-Документы по обозначениям' /app/'Ниокр-Документы по обозначениям'
COPY ./'Ниокр-Извещения об изменении' /app/'Ниокр-Извещения об изменении'
WORKDIR /app
RUN apt-get update && apt-get install -y qtbase5-dev cmake && cd /app/create_content_for_web/build && rm CMakeCache.txt && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j8 all
WORKDIR /app/create_content_for_web/build
CMD ["./createWebContent"]
