update account set salt = '', verifier = '';

alter table account modify column salt varchar(64) not null;
alter table account modify column verifier varchar(64) not null;

insert into build_info (build, majorVersion, minorVersion, bugfixVersion) values (18414, 5, 4, 8);