### Построение AST (Abstract Syntax Tree)  

__Быстрая сборка и генерация результатов__:
```bash
make && python launcher.py all
```  

__Пошаговая генерация результатов__:
```bash
# генерирует парсер по грамматике, собирает решение
make

# парсит файлы из директории data/Samples/, строит AST, сохраняет в формате DOT в results/SamplesGV/
python launcher.py dot

# компилирует файлы в results/SamplesGV/ в картинки (SVG), сохраняет в results/SamplesAST/
python launcher.py images

# генерирует код, аналогичный коду исходной программы, сохраняет в results/SamplesCode/
python launcher.py code
```

__Удаление всех сгенерированных файлов__:
```bash
# удаляет все результаты из директорий results/SamplesGV/, results/SamplesAST/, results/SamplesCode/
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
