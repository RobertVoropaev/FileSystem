Проект к курсу по Linux, 6 семестр

Воропаев Роберт, 691 группа, ФПМИ МФТИ

По расположению:

*settings.h  - настроки ФС

+core/* - реализация базовых структур: блоки, inode_table, bitmap и директории

+commands

  +str_proc.h - разнообразная обработка строк (преобразования для хранения в структурах, выделение папки и файла в пути и т.д.)
  
  +dir_inst.h - "низкоуровневые" функции для работы со структурой директории
  
  +base.h - функции по основному созданию и удалению файла (с выделением блока, добавлением в папку и т.д.)
  
  +main.h - реализация mkdir, touch и т.д., как оберток над базовыми функциями с дополнительной обработкой путей и т.д.
  
  +initial.h - создание и загрузка ФС
  
  +run.h - главная функция, обрабатывающая пользовательский ввод и запускающая соответствующие фунции из ./main.h
 
