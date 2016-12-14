### Построение AST (Abstract Syntax Tree)  

__Быстрая сборка и генерация результатов__:
```bash
make && python launcher.py all
```  

__Пошаговая генерация результатов__:
```bash
# генерирует парсер по грамматике, собирает решение
make

# парсит файлы из директории data/SamplesCorrect/, строит AST, сохраняет в формате DOT в results/SamplesCorrectGV/
python launcher.py dot

# компилирует файлы в results/SamplesCorrectGV/ в картинки (SVG), сохраняет в results/SamplesCorrectAST/
python launcher.py images

# генерирует код, аналогичный коду исходной программы, сохраняет в results/SamplesCorrectCode/
python launcher.py code

# выводит на экран ошибки компилятора, запущенного на файлах из data/SamplesCorrect/
python launcher.py code
```

__Удаление всех сгенерированных файлов__:
```bash
# удаляет все результаты из директорий results/SamplesCorrectGV/, results/SamplesCorrectAST/, results/SamplesCorrectCode/
python launcher.py clean

# удаляет все объектные и исполняемые файлы
make clean
```  

__Дополнения__:

При желании можно включить подробный вывод у визиторов. Для этого замените в вызовах `AstToCode` и `AstToDotLanguage` второй аргумент с `false` на `true`:
```c++
traversal = AstToCode( astRoot.get(), true );
// ...
traversal = AstToDotLanguage( astRoot.get(), true );
```    

Качать актуальную версию flex отсюда: https://sourceforge.net/projects/flex/files/
