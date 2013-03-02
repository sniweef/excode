from django.db import models
from django.contrib.auth.models import User

# Create your models here.
class Link(models.Model):
    url = models.URLField(unique=True)

class Bookmark(models.Model):
    title = models.CharField(max_length=20)
    #user = models.ForeighKey(User)
    #link = models.ForeighKey(Link)

