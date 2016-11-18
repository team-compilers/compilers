### Построение AST (Abstract Syntax Tree)  

Последовательность команд:  

```bash
# генерирует парсер по грамматике, собирает решение
make

# парсит файлы из директории data/Samples/, строит AST, сохраняет в формате DOT в data/SamplesGV/
python launcher.py dot

# компилирует файлы в data/SamplesGV/ в картинки (SVG), сохраняет в data/SamplesAST/
python launcher.py images

# генерирует код, аналогичный коду исходной программы, сохраняет в data/SamplesCode/
python launcher.py code

# удаляет все результаты из директорий data/SamplesGV/, data/SamplesAST/, data/SamplesCode/
python launcher.py clean
```  

При желании можно включить подробный вывод у визиторов. Для этого замените в вызовах `AstToCode` и `AstToDotLanguage` второй аргумент с `false` на `true`:
```c++
traversal = AstToCode( astRoot.get(), true );
// ...
traversal = AstToDotLanguage( astRoot.get(), true );
```    

Также можно после сборки (`make`) генерировать все результаты одной командой:
```bash
python launcher.py all
```

Качать актуальную версию flex отсюда: https://sourceforge.net/projects/flex/files/
