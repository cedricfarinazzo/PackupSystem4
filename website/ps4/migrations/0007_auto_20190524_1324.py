# Generated by Django 2.1.7 on 2019-05-24 11:24

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('ps4', '0006_auto_20190521_1448'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='archivecontent',
            name='backupfile',
        ),
        migrations.AddField(
            model_name='archivecontent',
            name='backup',
            field=models.ForeignKey(default=None, on_delete=django.db.models.deletion.CASCADE, to='ps4.Backup'),
            preserve_default=False,
        ),
    ]