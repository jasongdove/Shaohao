update account set salt = '', verifier = '';

alter table account modify column salt varchar(64) not null;
alter table account modify column verifier varchar(64) not null;
