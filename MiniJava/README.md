### Компилятор языка MiniJava на C++

__Быстрая сборка и генерация результатов__:
```bash
sudo apt install g++ python flex bison graphviz
make && python launcher.py all
```  

__Пошаговая генерация результатов__:
```bash
# сборка решения
make

# компиляция файлов из директории data/, генерация результатов в директории results/
python launcher.py run

# генерация изображений в директории results/ на основе graphviz-файлов (.gv)
python launcher.py images
```

__Очистка директории__:
```bash
# удаление всех результатов из директории results/
python launcher.py clean

# удаление всех объектных и исполняемых файлов
make clean
```  

__Дополнения__:
```bash
# последовательный запуск утилиты valgrind на всех файлах из директории data/
python launcher.py valgrind

# запуск компилятора на одном семпле
# NB! компилятор принимает два аргумента: компилируемый файл и директорию,
# в которой необходимо сгенерировать результаты компиляции
./minijava data/SamplesCorrect/Simple.java results/SamplesCorrect/Simple/

```
