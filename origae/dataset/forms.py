from __future__ import absolute_import

from flask.ext.wtf import Form
from wtforms.validators import DataRequired

from origae import utils


class DatasetForm(Form):
    """
    Defines the form used to create a new Dataset
    (abstract class)
    """

    dataset_name = utils.forms.StringField(u'Dataset Name',
                                           validators=[DataRequired()]
                                           )

    group_name = utils.forms.StringField('Group Name',
                                         tooltip="An optional group name for organization on the main page."
                                         )
