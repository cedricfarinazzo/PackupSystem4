# Generated by Django 2.1.7 on 2019-05-21 12:48

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('ps4', '0005_archivecontent'),
    ]

    operations = [
        migrations.AlterField(
            model_name='backupfile',
            name='file_type',
            field=models.CharField(choices=[('Archive part', 'Archive part'), ('Public key', 'Public key'), ('Private key', 'Private ley'), ('Lz78 Dictionnary', 'Lz78 Dictionnary')], default='Archive part', max_length=42),
        ),
    ]