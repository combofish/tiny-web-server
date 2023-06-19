use test;

set FOREIGN_KEY_CHECKS=0;
drop table if exists `student`;
create table `student` (	
	`id` tinyint not null auto_increment comment '学生id',
	`name` varchar(32) default null comment '学生名称',
	`age` smallint default null comment '年龄',
	`settime` datetime not null comment '入学时间',
	primary key (`id`)
) engine=InnoDB DEFAULT charset=utf8;


begin;
insert into `student` values(1,'张三',23,'2020-09-30 14:18:32');
insert into `student` values(2,'李四',22,'2020-09-30 15:18:32');
commit;

set foreign_key_checks = 1;


-- select * from student;