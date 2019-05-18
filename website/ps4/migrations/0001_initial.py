# Generated by Django 2.1.7 on 2019-05-18 19:15

from django.conf import settings
from django.db import migrations, models
import django.db.models.deletion
import ps4.models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
    ]

    operations = [
        migrations.CreateModel(
            name='Backup',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('enc_type', models.CharField(choices=[('None', 'None'), ('Rotn', 'Rotn'), ('Vigenere', 'Vigenere'), ('AES', 'AES'), ('RSA', 'RSA'), ('Elgamal', 'Elgamal')], default='None', max_length=11)),
                ('backup', models.FileField(upload_to=ps4.models.make_filepath)),
                ('filename', models.TextField(blank=True, null=True)),
                ('date_created', models.DateTimeField(auto_now_add=True)),
                ('user', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to=settings.AUTH_USER_MODEL)),
            ],
        ),
        migrations.CreateModel(
            name='StorageUser',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('usage', models.PositiveIntegerField(default=0)),
                ('user', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to=settings.AUTH_USER_MODEL)),
            ],
        ),
    ]
