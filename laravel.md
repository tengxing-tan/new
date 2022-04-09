## first time

```bash
composer update

# if error said iconv extension is disabled.
# go to '/etc/php/php.ini' find 'iconv', uncomment it
```

## Make model

```bash
php artisan make:model SingularName -a
```

## Common Data Types

```php
integer('column_name'); // integer
string('name', 100); 	// varchar
text('description'); 	// text
float('expense');		// float
softDeletes();			// delete_at ,it is timestamp

/**
	Foreign Key
*/
$table->foreignKey('column_name')->constrained()->nullable()->comment("leave comment here");
```

