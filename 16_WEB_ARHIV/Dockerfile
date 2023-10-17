FROM gcc:latest
COPY ./CONTENT /app/CONTENT
COPY ./create_content_for_web /app/create_content_for_web
#COPY ./Ниокр-Актуальные_документы /app/Ниокр-Актуальные_документы
#COPY ./Ниокр-Документы_по_обозначениям /app/Ниокр-Документы_по_обозначениям
#COPY ./Ниокр-Извещения_об_изменении /app/Ниокр-Извещения_об_изменении
#COPY ./Ниокр-Неактуальные_документы /app/Ниокр-Неактуальные_документы
WORKDIR /app
RUN apt-get update && apt-get install -y qtbase5-dev cmake p7zip-full gdb && cd /app/create_content_for_web/build && rm CMakeCache.txt && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j8 all
WORKDIR /app/create_content_for_web/build
CMD ["./createWebContent"]
