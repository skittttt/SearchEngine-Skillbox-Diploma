# Реализация локального поискового движка по файлам
## Короткое описание проекта
Программа представляет из себя консольное приложение, которое осуществляет поиск и имеет возможность настройки через файлы формата
JSON. Применённые в проекте решения можно впоследствии встроить в поисковой движок работающий на веб.

Реализованы следующие принципы работы:
1. В конфигурационном файле перед запуском приложения задаются названия
файлов, по которым программа будет осуществлять поиск.
2. Программа самостоятельно обходит все файлы и индексирует их так, чтобы потом по любому поисковому запросу находить наиболее
релевантные документы.
3. Пользователь задаёт запрос через JSON-файл requests.json. Запрос — это
набор слов, по которым нужно найти документы.
4. Запрос трансформируется в список слов.
5. В индексе ищутся те документы, в которых встречаются все эти слова. 
6. Результаты поиска ранжируются, сортируются и отдаются пользователю,
максимальное количество возможных документов в ответе задаётся в конфигурационном файле.
7. В конце программа формирует файл answers.json, в который записывает
результаты поиска.

## Примеры файлов конфигурации
### config.json
```json
{
    "config":
    {
    "name": "SkillboxSearchEngine",
    "version": "0.1",
    "max_responses": 5
    },
    "files":
    [
        "../resources/file001.txt",
        "../resources/file002.txt",
        "../resources/file003.txt",
        "../resources/file004.txt",
    ]
}
```
&emsp;config — общая информация\
&emsp;name — поле с названием поискового движка\
&emsp;version — поле с номером версии поискового движка.\
&emsp;max_responses — поле, определяющее максимальное количество ответов на один запрос.\
&emsp;files - содержит пути к файлам, по которым необходимо осуществлять поиск.\
&emsp;Внутри списка files лежат пути к файлам.\
&emsp;Впоследствии по содержимому файлов необходимо совершить поиск.

### Файл с запросами requests.json.
Cодержит запросы, которые необходимо обработать поисковому движку.\
Пример описания файла requests.json:\
```json
{
"requests": [
    "some words..",
    "some words..",
    "some words..",
    "some words..",
    ]
}
```
&emsp;“some words” — поисковый запрос, набор слов, разделённых одним или несколькими пробелами.
По ним осуществляется поиск.

## Используемые технологии
![GitHub author](https://img.shields.io/badge/C++-14-005199?style=for-the-badge)<br>

![GitHub author](https://img.shields.io/badge/CMake-3.22-005199?style=for-the-badge)<br>
https://cmake.org/<br>

![GitHub author](https://img.shields.io/badge/JSON-3.10.5-orange?style=for-the-badge)<br>
https://github.com/nlohmann/json<br>

![GitHub author](https://img.shields.io/badge/GTest-1.11.0-green?style=for-the-badge)<br>
https://github.com/google/googletest<br>

## Инструкции по установке
1. Создайте соответствующую директорию
2. Распакуюте в нее SearchEngine.zip 
3. Выполните процедуру компановки (компиляция и линковка) в соответствии с вашей операционной системой.
4. Сформируйте файл запросов(requests.json) в соответствии со стандартами json. (Запросы, помещенные в " " разделены пробелами)
5. Поместите в директорию resources набор файлов, в которых будет происходить поиск по запросам из requests.json
6. Сформируйте файл config.json в котором в разделе "files:" перечислите имена файлов из директории resources
7. Получите файл answers.json, в котором лежат результаты поиска
