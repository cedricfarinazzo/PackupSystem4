# Generated by Django 2.1.7 on 2019-05-20 21:50

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('ps4', '0003_backupfile_user'),
    ]

    operations = [
        migrations.AddField(
            model_name='backup',
            name='comp_type',
            field=models.CharField(choices=[('None', 'None'), ('Huffman', 'Huffman'), ('Lz78', 'Lz78')], default='None', max_length=11),
        ),
    ]
